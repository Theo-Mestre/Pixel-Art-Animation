uniform sampler2D u_texture;
uniform vec4 u_clearColor;

void main()
{
    vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);

    if (pixel.a == 0.0)
    {
        gl_FragColor = u_clearColor;
        return;
    }

    gl_FragColor = pixel * gl_Color;
}