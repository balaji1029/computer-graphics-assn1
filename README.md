# CS675 Assignment 1 Submission by 23B1029+23B1081

## Team Members

- Balaji Karedla (23B1029)
- Rishi Kalra (23B1081)

## Declaration

We declare that we have written the assignment code ourselves.
Sources we have taken help/assistance from:

OpenGL Tutorials from https://github.com/paragchaudhuri/cs475-tutorials

ChatGPT for clarifying concepts (not for writing full code)

## Build Instructions


## Build the project
Run the following command when in the root directory of the project:
```bash
make
```
## Run the modeller
Run the following command when in the root directory of the project, after building:
```bash
./bin/main
```

## Keymaps

### Modes
```
M → Switch to Modelling mode

I → Switch to Inspection mode

Esc → Exit the program
```
### Modelling Mode

<!-- add a table for the keymaps -->

By default the window is in Inspection mode. To Go to Modelling mode, press 'M'.
| Key(s)     | Action                                      |
|------------|---------------------------------------------|
| **1**      | Add Sphere                                  |
| **2**      | Add Cylinder                                |
| **3**      | Add Box                                     |
| **4**      | Add Cone                                    |
| **5**      | Remove last shape                           |
| **D**      | Duplicate the current shape                 |
| **R**      | Enter rotation mode                         |
| **X / Y / Z** | Select axis (rotation/translation/scaling) |
| **+ / -** or **UP / DOWN**  | Rotate around chosen axis  |
| **T**      | Enter translation mode                      |
| **+ / -** or **UP / DOWN** | Translate along chosen axis |
| **G**      | Enter scaling mode                          |
| **+ / -** or **UP / DOWN**  | Scale along chosen axis    |
| **C**      | Change colour (enter RGB on terminal)       |
| **S**      | Save model to `.mod` file                   |
| **V**      | To change Zoom |
|**+ / -** or **UP / DOWN**    | Zoom In/Out respectively |

### Inspection Mode

| Key(s)     | Action                                      |
|------------|---------------------------------------------|
| **L**      | Load model from .mod file                   |
| **R**      | Rotate entire model (same as above)         |
| **V**      | To change Zoom (same as above)              |

## Instructions

The default mode is Inspection mode. To switch to Modelling mode, press 'M'.

For adding shapes, press the number corresponding to the shape you want to add. The shape will be added at the origin with default size and colour. The shape added later will be selected by default and the parent of the shape will be the previously added shape.

The axes of the selected shape are same as its parent's axes.

While saving, specify the filename with .mod extension. The model will be saved in the models directory of the project.
While loading, specify the filename (with path relative to the models directory) with .mod extension. The model will be loaded from the models directory of the project.

## Saved files:

The models are included in the root directory as model1.mod and model2.mod

model1.mod

 ![alt text](model1.png)

model2.mod

 ![alt text](model2.png)


[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/FRxE5qBZ)
