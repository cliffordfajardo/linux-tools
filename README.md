# linux-tools
My own versions of GNU coreutils programs plus a few others, written for fun and profit as I learn about Linux system programming. All programs I haven't implemented yet are listed in `TODO.md`.

# Building
Each subdirectory has a `Makefile` in it that should handle compiling the code and executing it. The `Makefile`s are
expected to be run from within the Dockerized workspace environment. Executing `make docker shell` should pull and build
the container, then drop you into an executable shell.
