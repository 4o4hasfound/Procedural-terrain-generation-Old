#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D waterTexture; 

uniform bool useTextures;

void main()
{    if (useTextures){
        FragColor = texture(waterTexture,TexCoord);
     }
     else{
        FragColor = vec4(0.11, 0.764, 0.858, 1.0);
     }
    
}