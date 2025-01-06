## Setup

```sh
apt install libglfw3-dev libglfw3-doc
```

This means I'll be able to link against `libglfw` (not `libglfw3`!).

```
root@computer:~# dpkg -L libglfw3-dev | fgrep .so
/usr/lib/x86_64-linux-gnu/libglfw.so
```

Then go to: https://gen.glad.sh/

- select gl v3.3
- compatibility set to "core" (on the right)
- select the "loader" checkbox (in the "options" section)
- click the "generate" button