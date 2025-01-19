uniform sampler2D u_texture;
uniform sampler2D u_animation;

void main()
{
    vec4 pixelCoord = texture2D(u_animation, gl_TexCoord[0].xy);
    vec2 coord = pixelCoord.xy;

    vec4 finalColor = texture2D(u_texture, coord + 0.01);

    gl_FragColor = gl_Color * finalColor * vec4(1, 1, 1, pixelCoord.a);
}