//
//  box.frag
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/23/18.
//
//

precision highp float;

#define BORDER_X 1.0
#define BORDER_Y 0.2

uniform float uTime;
uniform vec2 uRes;
uniform int uBorder;
uniform float uAlphaMult;
uniform vec4 globalColor;

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
	vec2 uv = vTexCoord.xy;
	
	// Compute the attenuation factors.
	vec2 fc = 1.0 - smoothstep(vec2(BORDER_X, BORDER_Y), vec2(1.0), abs(2.0*uv-1.0));
    float fact = fc.x * fc.y;
 
    gl_FragColor.rgb = vec3(0.0, 0.0 , 0.0 ) + globalColor.rgb ;
    gl_FragColor.a = fact*(1.0 - uAlphaMult);
}
