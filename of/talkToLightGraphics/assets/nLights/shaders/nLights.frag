//
//  nLights.frag
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/21/18.
//
//

#version 120

uniform float uTime;
uniform vec2 uRes;
uniform sampler2D uNoise1;
uniform sampler2D uNoise2;
uniform float uPulseMult;
uniform vec3 uColor;
uniform bool uRespond;

//varying vec2 texCoordVarying;

#define TAU 6.2831853071

float distY(vec2 uv){
    //float o = texture2DRect(uNoise1, texCoordVarying).r;
    //float o = texture(uNoise1, uv * 0.25 + vec2(0.0, uTime)).g;
    //uv.y += sin(uv.x * 40.0) * 0.05;
    
    //uv.y += texture2D(uNoise1, vec2(0.0, uv.r )).r * 0.10;
    
    float noiseY_1 = texture2D(uNoise1, vec2(0.0, uv.r )).r ;
    float noiseY_2 = texture2D(uNoise2, uv).r ;
    
    float noiseY = 2.0*noiseY_1 + 0.2*noiseY_2;
    //noiseY = noiseY_1;
    //noiseY = step(0.5,noiseY);
    
    float v;
    v = uv.y ;//* noiseY *0.05 * sin(uTime) ;
    if(uv.y > 0.5){
        v = 1.0 - abs(v * 2.0 - 1.0);
        v = pow(v, 1.0 + sin((uTime * 0.25 + noiseY * 0.5) * TAU) * 0.5); // pulse
    }else{
        v = 1.0 - abs(v * 2.0 - 1.0); // not producing streaks at the bottom
    }
    return v;
}


float addStars( vec2 seed, float v, vec2 uv){
    vec2 r;
    
    if(uv.y > 0.5)
    {
        r.x = fract(sin((seed.x * 12.9898) + (seed.y * 78.2330)) * 43758.5453);
        r.y = fract(sin((seed.x * 53.7842) + (seed.y * 47.5134)) * 43758.5453);
        
        float stars = mix(r.x, (sin((uTime * 5.0 + 60.0) * r.y) * 0.5 + 0.5) * ((r.y)), 0.04);
        float starsColor = pow(stars,50.0) * (1.0 - v);
        
        return starsColor;
    }
    else
        return 0.0; // if bottom half of the screen do not add stars
    
    
}

vec3 addAurora( vec2 uv, float v ){
    
    //uv.y += sin(uv.x * 40.0) * 0.05;
    
    float x, y;
    if(uv.y > 0.5)
    {
        if(uRespond){
            x = sin( (uv.x * .5) * uTime * 10.0) ;
        }else{
            x = (1.0 - uv.x);
        }
    }
    y = 1.0 - abs(uv.y * 2.0 - 1.0);
    
    //return vec3(x, y, x) * v;
    return uColor * v * vec3( x, y, 1.0 );
    
}

float alphaMasking(vec2 uv){
    vec2 fc = 1.0 - smoothstep(vec2(uPulseMult), vec2(1.0), abs(2.0*uv-1.0));
    float alpha = fc.x * fc.y;
    return ( alpha + 0.15);
}

void main(){
    
    vec2 uv = gl_FragCoord.xy / uRes.xy;
    
    uv.y += 0.2;
    
    // noisy curve
    float v = distY( uv );
    
    // adding  aurora
    vec3 color = vec3( 0.0 );
    color += addAurora( uv, v );
    
    //adding stars
    //vec2 seed = gl_FragCoord.xy;
    //color += addStars( seed, v , uv);
    
    //float noiseY = texture2DRect(uNoise1, uv * 100 + vec2(0.0, uTime)).g ;
    //color = vec3(noiseY, 0,0);
    gl_FragColor.rgb = color;
    
    gl_FragColor.a = alphaMasking(uv) ;//* uv.y;
}
