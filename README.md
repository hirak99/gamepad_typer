Simple binary to allow typing with Steam Deck gamepad on console

# Building

From the root, run -

`make -j`

This will create `build/gamepad_typer`. It can be used to ask for inputs from a
shell script.

Here is how it can be used in a shell script -

```sh
#!/bin/bash
PASS=$(/path/to/gamepad_typer)
echo Password is: "$PASS"
```
