//
//  box.frag
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/23/18.
//
//

#version 120

#define BORDER_X 0.95
#define BORDER_Y 0.8

uniform float uTime;
uniform vec2 uRes;
uniform int uBorder;
uniform float uAlphaMult;

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
	vec2 uv = vTexCoord.xy;

	// Compute the attenuation factors.
	vec2 fc = 1.0 - smoothstep(vec2(BORDER_X, BORDER_Y), vec2(1.0), abs(2.0*uv-1.0));
    float fact = fc.x * fc.y;

    gl_FragColor.rgb = vColor.rgb ;
    //if(uBorder > 0){
    	gl_FragColor.a = (fact) * uAlphaMult;
    //}
    //else
    //	gl_FragColor.a = 1.0 * uAlphaMult;
}
