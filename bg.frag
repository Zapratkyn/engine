#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D texture1;
// uniform float spacingFactor;
// uniform float patterWidth;

void main()
{
    // float scaledX = TexCoord.x * 2.0;

    // float unitX = fract(scaledX);

    // if (unitX > 0.5)
    //    discard;

    // vec2 sampledUV = vec2(unitX / 0.5, TexCoord.y);
    FragColor = texture(texture1, TexCoord);
}
