precision highp float;

uniform vec2 windowSize;
uniform vec2 rectPixelSize;
uniform vec2 totalPixelSize;
uniform vec2 rectSize;
uniform vec2 glowSize;
uniform vec4 glowColor;

varying vec4 vColor;
varying vec2 vTexCoord;

float rectangle( vec2 p, vec2 b )
{
    return length(max(abs(p) - b, 0.0));
}

float roundRect( vec2 p, vec2 b, float r )
{
    return length(max(abs(p) - b, 0.0)) - r;
}

float glowRect(vec2 texcoords)
{
    // in pixel coords
    //    vec2 uv = texcoords * totalPixelSize;
    //    vec2 halfRes = 0.5 * totalPixelSize;
    //    float radius = 5.0;
    //    float border = 15.0;
    //
    //    // compute box
    //    return 1.0 - roundRect( uv - halfRes, halfRes - border, radius ) * 1.15;
    
    // in texcoords
    vec2 ratio = vec2(totalPixelSize.x / totalPixelSize.y, 1.0);
    vec2 uv = texcoords * ratio;
    vec2 halfRes = vec2(0.5) * ratio;
    float radius = 0.0 / totalPixelSize.y;
    vec2 border = (vec2(15.0) / totalPixelSize.xy) * ratio;
    
    // compute box
    return 1.0 - roundRect( uv - halfRes, rectSize * ratio * 0.5, radius ) * 2.0;
}

void main()
{
    float ratio = vTexCoord.x / vTexCoord.y;
    vec2 uv = vTexCoord * 2.0 - 1.0;
    
    float rect = 1.0 - rectangle(uv, rectSize) * 20.0;
    float glow = glowRect(vTexCoord) * 0.5;
    rect = clamp(rect, 0.0, 1.0);
    glow = clamp(glow, 0.0, 1.0);
    
    gl_FragColor = mix(glowColor, vColor, rect);
    gl_FragColor.a *= max(rect, glow);
    
}

