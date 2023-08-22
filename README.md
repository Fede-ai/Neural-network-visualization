# Neural-network-visualization

In this SFML and C++ project I've implemented a simple neural network using the sigmoid activation function and a different bias for each node. I've also added a 200x135 pixels image created using the neural network.

### Change weights/biases values
You can change the values of the network's biases and weights through some sliders at the left of the screen. You can scrool through the sliders using the mouse wheel. The top 2 sliders change the values fed to the neural network when it is drawn (the don't affect the image). 
#### How to read the weights coordinates
The first number of a weight's coordinates indicates the layer the weight starts from (input layer = layer n. 1). The second number indicated which node the weight starts from (top most node = node n. 1). The third number tells you on which node does the weight end in the following layer (top most node = node n. 1).
#### How to read the biases coordinates
The first number of a bias' coordinated indicates the layer of the bias' node (input layer = layer n. 1) while the second number indicated the node position inside  that layer (top most node = node n. 1).

### Info about the image
By pressing the space button you can toggle the neural network with an image created by feeding the coordinates of each pixel to the neural network.
The top left corner of the image corresponds to the coordinates [-5, -5], while the bottom right corner to corresponds to [5, 5].

### The network's size
The first layer has a fixed size of 2 nodes while the last one has eighter a size of 2 nodes (if you want a black and white image) or a size of 3 nodes (if you want a colored image).
You can decide the size of the neural network at the start of the program (the process is explained step-by-step in the console). 

### Outputs usage
In the black and white mode: if the first output is higher than the second the pixel is set to white, else, it is set to black. In the colored mode: each output is multiplied by 255; the values obtained represent the red, green and blue values of the pixel.

### How to move around
You can use the mouse wheel to zoom and you can left click and move the cursor to move up, down, left and right. 
By pressing 'C' you will automatically place the neural network/image at the center of the screen. 
Like most applications, pressing 'alt' and 'enter' together will toggle fullscreen.

<!-- release, console commands -->
