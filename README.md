# learn-opengl-tutorial

Following along: https://learnopengl.com/Getting-started/Hello-Triangle

## Setup


```sh
# Provides glslangValidator required by the "GLSL Lint" vscode plugin
apt install glslang-tools
```

This post was really useful to get the project setup:

https://www.reddit.com/r/opengl/comments/1hu7ti3/minimal_opengl_project_setup_in_vs_code_on_windows/

Now, just use CLion or vscode and with the right extension, everything works
to be productive and is self-contained.

---

If getting `AddressSanitizer:DEADLYSIGNAL` in a loop, do this to make working with the sanitizer possible:

```sh
# As root
sysctl vm.mmap_rnd_bits # read
sysctl -w vm.mmap_rnd_bits=28 # write (reduce from 32 on ubuntu 22.04)
```

Source: https://stackoverflow.com/a/77994596
