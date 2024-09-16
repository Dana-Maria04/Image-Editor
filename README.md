# Image-Editor

// Copyright Caruntu Dana-Maria 311CAa 2023-2024

    This project aims to function as an image editor for both grayscale and RGB photographs in PGM or PPM formats, following commands entered from the keyboard:

-> Loading an image into memory = LOAD
-> Selection by desired coordinates = SELECT <x1, y1, x2, y2>
-> Select the entire image = SELECT ALL
-> Rotating the image = ROTATE <angle>
-> Equalizing the grayscale tones = EQUALIZE
-> Cropping an image = CROP
-> Applying filters to the image = APPLY <parameter>
-> Saving the image = SAVE <filename>
-> Generating a histogram for RGB images = HISTOGRAM <x,y>
-> Exiting the program = EXIT

    Additionally, if any command other than those listed above is entered, the program will display "Invalid command."

    Another message that may be displayed is "No image loaded" if no image has been loaded when a command is called.

    For error messages involving commands with multiple parameters, I have checked whether they are valid using a character string that produces the following word.

    The source code is based on .c files and .h headers, which contain dynamic allocations and deallocations of memory, image structure, command code, and their calls. The "image-struct.h" file contains the "image" structure, which includes information about the image such as length, width, coordinates x1, y1, x2, y2, its magic-word (P2/P3/P4/P5), and whether it has been loaded (nr_img). The "image" structure contains both the grayscale image declared as type int and the RGB image, using the "rgb" structure with red, green, and blue colors. Memory allocations and deallocations are performed separately for grayscale and RGB images.

    In the main source code, I used a character string "command" to start comparing with the given commands and to determine which functions to call.

#### LOAD

   This command loads the desired image if it exists. If an image is already loaded, it is deallocated to make room for the new image. The "LOAD" command reads grayscale and RGB images in both binary and ASCII formats. Using the "magic-word" variable in the image structure, I can compare with the desired image types to determine which one to LOAD. Finally, the "nr_img" variable in the image structure retains whether an image has been loaded or not, taking the value 1. Through this, I can check to display the message "No image loaded."

#### SELECT <x1, y1, x2, y2>

    Following this command, if all coordinates are valid, and if x1 > x2 or y1 > y2, they are swapped to achieve the correct selection and are assigned accordingly. For the SELECT command, I read whether the continuation of the command is coordinates or "ALL" for a full selection.

#### SELECT ALL

    Updates the coordinates to those corresponding to the entire image: x1 and y1 become 0, and x2 and y2 become the length and width of the image.

#### ROTATE <angle>

    For ROTATE, there is a single function that performs a left rotation, counterclockwise, which is called multiple times depending on the desired angle. To ensure the desired angle is one of the possible values (+/-0, +/-90, +/-180, +/-270, +/-360), I created a vector with these values that is checked by an "ok_angle". If the value is found, depending on the image type, angle, and selection, the rotation is performed. Thus, the image can be rotated for a specific selection (only if it is square) as well as for the entire selection. The basis for Rotate consists of two functions, "ROTATE_LEFT_SELECT" and "ROTATE_LEFT_SELECT_ALL", to handle the two selection cases. In the "ROTATE_LEFT_SELECT" function, two auxiliary matrices are used, "selected", which holds the desired selection for rotation, and "rotated", which is used in the rotation process and will be deallocated at the end of the process. If the requested rotation angle is not among the possible values, the message "Unsupported rotation angle" will be displayed.

#### EQUALIZE

    This function aims to equalize the grayscale tones in an image, but only if it is grayscale. It is based on the following formula:

        f(a) = 255 * (1/ Area) * (sum (i=0 to a) H(i))

#### CROP

    Cropping is performed using a temporary matrix (for both grayscale and RGB) that holds the desired selection for the crop, and the dimensions are updated to those in the crop selection. The auxiliary matrix "cropped" updates the image to the one obtained after cropping, deallocating it at the end of the process.

#### APPLY <parameter>

    This command aims to apply filters such as Edge, Sharpen, Blur, Gaussian Blur. It uses a kernel represented as a 3x3 matrix, whose values are multiplied by the elements in the RGB image, using the formula:

      img->photo[i-m-1][j-n-1].R*kernel[m][n];

      img->photo[i-m-1][j-n-1].G*kernel[m][n];

      img->photo[i-m-1][j-n-1].B*kernel[m][n];

    These values are placed in an auxiliary matrix through which the final image is updated. The values of R, G, and B are also subjected to the "clamp" function to ensure that their values are within the [0,255] range. If the loaded image is grayscale, the message "Easy, Charlie Chaplin" will be displayed. Along with this message, I have also used the "charlie" variable in the image structure that acts as an "ok", and if the message is displayed, it becomes 1. Through this method, the other message, "Apply parameter invalid," cannot be displayed unless the filter to be applied is not one of the four mentioned at the beginning.

#### SAVE <filename>

    This command saves the image, both in binary and ASCII formats, printing its magic-word, maximum value, length, width, and image. Opening the file and writing to it are done defensively. Additionally, the "ascii" variable in the image structure acts as an "ok" to verify whether the desired save format is ASCII or not, to determine how the image should be saved.

#### HISTOGRAM <x,y>

    The "HISTOGRAM" function divides the range [0,255] into y groups to determine the number of "*". In the frequency vector "freq", I store the number of occurrences of each element in the matrix, which is used to build the histogram.

#### EXIT

    This function exits the program and deallocates all memory. If no image was loaded, it only deallocates the structure. If an RGB image was loaded, it frees it using the "free_rgb" function, and if it is grayscale, it frees it using the "free_matrix" function.
