# Pendulum 

Position based dynamics example using triple pendulum. 

## Build

Use `make` with the name of the program you wish to create.
If no name is provided will create `pendulum`.

## `pendulum`

`pendulum` outputs a `csv` of 10 seconds of simulation.
If a filename is not provided `pendulum` outputs to standard output. 

## `vid`
`vid` turns a `csv` from `pendulum` into `mp4`. 
First argument is the source `csv`.
If this argument is not provided default to standard input.
Second argument is the output `mp4`.
If this argument is not provided default to `pendulum.mp4`.

