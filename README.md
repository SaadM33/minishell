# minishell

A small Bash-like shell written in C (42 school project). It reads a line with
GNU Readline, parses it, and runs the resulting pipeline. The interactive
prompt is `Megashell> `.

## Features

- **Builtins:** `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **External commands:** looked up through `$PATH`, or run directly when the
  name contains a `/`
- **Pipes:** `cmd1 | cmd2 | cmd3`, each stage in its own process
- **Redirections:** `<`, `>`, `>>`, and heredoc `<<`
- **Quoting:** single quotes (literal) and double quotes (expansion only)
- **Environment expansion:** `$VAR` and `$?`
- **Signals:** `Ctrl-C`, `Ctrl-\` and `Ctrl-D` behave as in an interactive shell
- **History** via Readline (`add_history`)

Not implemented: `&&`, `||`, `;`, subshells `( )`, wildcards (`*`), `${VAR}`,
`$$`, backslash escapes, and running a script file (the shell is always
interactive-style: it reads lines from stdin).

### Builtin notes

| Builtin  | Behaviour |
|----------|-----------|
| `echo`   | Prints its arguments separated by a space; leading `-n`, `-nn`, ... arguments suppress the trailing newline. |
| `cd`     | `cd` (no args) goes to `$HOME`, `cd -` goes to `$OLDPWD`; more than one argument is an error. Updates `PWD` and `OLDPWD`. |
| `pwd`    | Prints the shell's tracked current directory. |
| `export` | `export KEY=value` sets a variable; invalid identifiers are reported and give status 1. With no arguments it prints the environment sorted, as `declare -x KEY="value"`. A name without `=` is only validated, not stored. |
| `unset`  | Removes variables from the environment. |
| `env`    | Prints the environment. |
| `exit`   | `exit [n]`: exits with `n` modulo 256, or with the last status when no argument is given. A non-numeric argument exits with 2; too many arguments is an error and the shell stays alive. |

A builtin that is the only command on the line runs in the shell process (so
`cd`, `export`, `unset` and `exit` take effect). Inside a pipeline each builtin
runs in a forked child, so its effects are lost when the pipeline ends.

### Quoting and expansion

- `'...'` — everything is literal.
- `"..."` — `$VAR` and `$?` are expanded, the result is not split into words.
- Unquoted — `$VAR` is expanded and the result is split on whitespace into
  separate arguments. A variable that expands to nothing produces no argument.
- Quoted and unquoted pieces that touch each other are joined into one word
  (`a"b"'c'` is `abc`).
- A `$` not followed by a letter, `_` or `?` is kept as a literal `$`.
- `$?` is the exit status of the last command.

### Redirections and heredoc

- Several redirections may appear in one command; they are processed left to
  right and the last output (and input) redirection wins. Earlier `>` files are
  still created/truncated.
- A redirection target that expands to zero or several words is reported as
  `ambiguous redirect`; a file that cannot be opened prints the system error.
  In both cases that command is skipped and the status is 1.
- `<< DELIM` reads lines with a `> ` prompt until a line equals `DELIM`. If the
  delimiter is quoted (`<< 'EOF'` or `<< "EOF"`), the body is not expanded;
  otherwise `$VAR` and `$?` are expanded. `Ctrl-D` ends the heredoc with a
  warning; `Ctrl-C` aborts it and the line is not run.

### Signals

| Where | `Ctrl-C` | `Ctrl-\` | `Ctrl-D` |
|-------|----------|----------|----------|
| At the prompt | New prompt on a fresh line, `$?` becomes 130 | Ignored | Prints `exit` and quits |
| While a command runs | Delivered to the child (default action) | Delivered to the child; prints `Quit (core dumped)` when it dies with signal 3 | n/a |
| In a heredoc | Aborts the heredoc (status 130) | Ignored | Ends the heredoc with a warning |

### Exit status

`$?` follows the usual conventions: the status of the last command in a
pipeline, `127` for command not found, `126` for permission denied or a
directory, `128 + n` when killed by signal `n`, `2` for syntax errors.

## Build and run

Requirements: a C compiler (`cc`), `make`, and the GNU Readline development
library (e.g. `libreadline-dev` on Debian/Ubuntu).

```sh
make          # builds libft/libft.a, then ./minishell
./minishell   # or: make run
```

Other Makefile targets:

| Target       | Effect |
|--------------|--------|
| `make val`   | Build and run under Valgrind with `readline.supp` (leaks, open fds) |
| `make clean` | Remove object files and clean libft |
| `make fclean`| `clean` plus remove the `minishell` binary |
| `make re`    | `fclean` then `all` |

The project is compiled with `-Wall -Wextra -Werror -g`.

Example session:

```text
Megashell> export NAME=world
Megashell> echo "hello $NAME" | tr a-z A-Z > out.txt
Megashell> cat < out.txt
HELLO WORLD
Megashell> cat << EOF
> user is $NAME
> EOF
user is world
Megashell> echo $?
0
```

## Project structure

```text
.
├── Makefile
├── readline.supp          # Valgrind suppressions for Readline
├── includes/minishell.h   # structs, macros, prototypes
├── src/
│   ├── main.c             # REPL loop, global state, ft_exit cleanup
│   ├── parsing/           # lexer, syntax check, heredoc, redirections, expansion
│   ├── exec/              # pipeline execution, PATH lookup, builtin dispatch
│   ├── built_in/          # cd, echo, env, exit, export, pwd, unset
│   └── utils/             # env helpers, token/command lists, errors, signals
├── libft/                 # in-house libc subset, get_next_line, ft_malloc/ft_free
└── docs/ARCHITECTURE.md   # how the pipeline works
```

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for a walk through the
lexer → parser → expander → executor pipeline.
