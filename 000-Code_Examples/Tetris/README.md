Before Running the project, please make sure

1. Make sure your working directory is set to your project root. Otherwise `images` folder may not work properly:

   Under `Run` > ` Edit Configurations...`

   For example: `E:\CLion_Projects\hello_sfml`

### Description & Explanation

The `figures` array describing the shape of puzzle according to the following table:

| 0 | 1 |
|---|---|
| 2 | 3 |
| 4 | 5 |
| 6 | 7 |

```
// Here is the coordinates of each puzzle:

        { 1, 3, 5, 7 },   // I
        { 2, 4, 5, 7 },   // Z
        { 3, 5, 4, 6 },   // S
        { 3, 5, 4, 7 },   // T
        { 2, 3, 5, 7 },   // L
        { 3, 5, 7, 6 },   // J
        { 2, 3, 4, 5 },   // O
```
