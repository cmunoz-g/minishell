# minishell
Welcome to our minishell! This project was completed by [cmunoz-g](https://github.com/cmunoz-g) and [jjuanramos](https://github.com/jjuanramos).

If you want context about the project, we recommend to read [maiadegraaf's README](https://github.com/maiadegraaf/minishell/), it is explained incredibly well so we thought it's redundant to re-do it.
- [tl;dr](#tldr)
- [Installation](#installation)
- [Commands to try](#commands-to-try)

## tl;dr

Still, If you want to dive straight in, the summary is that this project aims to recreate a mini version of [bash](https://en.wikipedia.org/wiki/Bash_(Unix_shell)).

The requirements are:
- Have a functional history of all previous commands.
- Run the right executable based on PATH or relative and absolute paths.
- Implement redirections such as <, >, «, and ».
- Implement pipes (|).
- Handle environment variables.
- Handle $?.
- Handle signals such as ctrl-C, ctrl-D, and ctrl-\.
- Implement the built-ins: echo, cd, pwd, export, unset, env, and exit (except echo and cd, all of them without arguments).
- Make sure there are no leaks.


## Installation

In order to execute minishell, you will need `gcc` installed. You'll probably have it installed by default, otherwise, we recommend you head to [GNU's website](https://gcc.gnu.org/install/) and install it.

With that out of the way, the steps needed to execute minishell are:
1. Cloning the repo: `git clone git@github.com:cmunoz-g/minishell.git`
2. `cd minishell`
3. `make`
4. `./minishell`

That should be it! It's been tested on Linux and MacOs.

## Commands to try

Get creative! Some examples are:

- `cat | cat | cat | echo << end`
- `ls -la | wc -c >> out.txt`
- `cat < out.txt`
- `echo $PATH >> out.txt`
- `export a=HELLO`
- `echo $HELLO`
- `echo '$HELLO'` 

And more! The heuristic is that, as long as the commands fit the [requirements](#tldr), they should behave as bash.