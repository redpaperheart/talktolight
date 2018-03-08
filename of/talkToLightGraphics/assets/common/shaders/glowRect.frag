#version 120

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
    float radius = totalPixelSize.x * 0.1;
    vec2 border = 0.5 * (totalPixelSize - rectPixelSize);
    float blur = border.y;
    
    vec2 uv = texcoords * totalPixelSize;
    vec2 halfRes = 0.5 * totalPixelSize;
    float iBlur = 1.0 / (1.0 + blur);
    float iRadius = radius;
    vec2 iBorder = border;
    
    // compute box
    // float b = roundRect( uv - halfRes, halfRes - iBorder - iRadius - blur, iRadius ) * iBlur;
    float b = roundRect( uv - halfRes, halfRes - iBorder - iRadius, iRadius ) * iBlur;
    return 1.0 - clamp(b, 0.0, 1.0);
}

void main()
{
    float ratio = vTexCoord.x / vTexCoord.y;
    vec2 uv = vTexCoord * 2.0 - 1.0;
    
    float rect = 1.0 - clamp(rectangle(uv, rectSize) * 200.0, 0.0, 1.0);
    float glow = glowRect(vTexCoord);// * 0.7;
    
    gl_FragColor = mix(glowColor, vColor, rect);
    gl_FragColor.a *= max(rect, glow);
}

