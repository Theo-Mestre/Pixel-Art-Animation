uniform sampler2D u_texture;
uniform sampler2D u_animation;

void main()
{
    vec4 pixelCoord = texture2D(u_animation, gl_TexCoord[0].xy);
    vec2 coord = pixelCoord.xy;

    vec4 finalColor = texture2D(u_texture, coord + 0.01);

    // Debugging output
    // gl_FragColor = vec4(gl_TexCoord[0].xy, 0, 1); // Visualize texture coordinates
    // gl_FragColor = pixelCoord; // Visualize pixelCoord
    //gl_FragColor = finalColor; // Visualize finalColor

    gl_FragColor = gl_Color * finalColor * vec4(1, 1, 1, pixelCoord.a);
}