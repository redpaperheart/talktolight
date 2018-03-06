precision highp float;

uniform sampler2D tex0;
uniform vec2 windowSize;

varying vec4 vColor;
varying vec2 vTexCoord;

void main()
{
    vec3 tex = texture2D(tex0, gl_FragCoord.xy / windowSize).rgb;
    gl_FragColor.rgb = tex;
    gl_FragColor.a = vColor.a;
}

