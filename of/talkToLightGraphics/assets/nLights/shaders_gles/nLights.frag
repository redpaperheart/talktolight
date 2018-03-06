//
//  nLights.frag
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/21/18.
//
//

precision highp float;

uniform float uTime;
uniform vec2 uRes;
uniform sampler2D uNoise1;
uniform sampler2D uNoise2;
uniform sampler2D uNoise3;
uniform sampler2D uNoise4;
uniform sampler2D uNoise5;
uniform float uPulseMult;
uniform vec3 uColor;
uniform bool uRespond;
uniform float uAlphaMult;
uniform float uAudio_1;
uniform float uAudio_2;
uniform float uAudio_3;


varying vec2 texCoordVarying;

#define TAU 6.2831853071

float distY(vec2 uv){
	

	//uv.y +=  0.5 * sin(uv.x * uTime ) * 0.02 ;
	//uv.y += texture2D(uNoise1, vec2(0.0, uv.r + 0.05)).r * 0.05;

  float noiseY_1 = texture2D(uNoise1, vec2(0.0, uv.r )).r ;
  
  float noiseY_2;
  if(uv.y > 0.5){
	noiseY_2 = texture2D(uNoise2, uv).r ;	
  }else{
	  noiseY_2 = 0.0;
  }
  
  float noiseY = 2.0*noiseY_1 + 0.25*noiseY_2;
  
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

  float x, y;
  if(uv.y > 0.5)
  {	
	x = (1.0 - uv.x);
  }
  y = 1.0 - abs(uv.y * 2.0 - 1.0);
    
  return uColor * v * vec3( x, y, 1.0 );

}

float alphaMasking(vec2 uv){
	vec2 fc = 1.0 - smoothstep(vec2(uPulseMult), vec2(1.0), abs(2.0*uv-1.0));
	float alpha = fc.x * fc.y;
	return ( alpha + 0.15);
}

void main(){
    vec2 uv = gl_FragCoord.xy / (uRes.xy);
    
    // uv.y += 0.2;

    // noisy horizon
    uv.y += 0.15 * texture2D(uNoise3 , vec2(uAudio_1 , uv.r )).r;
    uv.y += 0.1 * texture2D(uNoise2 , vec2(uAudio_2 , uv.r )).r;
    uv.y += 0.15 * texture2D(uNoise5 , vec2(uAudio_3 , uv.r )).r;
     
    //displace u v.y for northern lights streaks 
    float v = distY( uv );

    // adding  aurora 
    vec3 color = vec3( 0.0 );
    color += addAurora( uv, v );

    //adding stars
    //vec2 seed = gl_FragCoord.xy;
    //color += addStars( seed, v , uv);

    gl_FragColor.rgb = color;
    gl_FragColor.a = uAlphaMult * alphaMasking(uv) ;

}
