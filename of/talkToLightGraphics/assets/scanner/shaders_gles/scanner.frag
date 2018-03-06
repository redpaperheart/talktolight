//
//  scanner.frag
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/26/18.
//
//

precision highp float;

uniform float uTime;
uniform vec2 uRes;

varying vec2 texCoordVarying;

float r = 0.8;
float g = 0.0;
float b = 0.8;

uniform float uBarWidth_1;
uniform float uBarWidth_2;
uniform float uRespBarWidth;
uniform float uBarDistance;
uniform bool  uTrigger;
uniform bool  uDrawResp;
uniform bool  uMoveUV;
uniform float uRectPosX_1;
uniform float uRectPosX_2;  
uniform float uRespBarPosX;

vec3 color1 = vec3(1.0, 0.0, 0.471); //first rect
vec3 color2 = vec3(0.784 , 0.0, 1.0); //second rect
vec3 color3 = vec3(1.0); // voice response

float map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}


float distY(float x){

  //float noiseY = texre2DRe1uNoise1, vec2(0.0,uv.r)).r ;

  //float v = uv.y + noiseY * 0.1;
  float v = x;
  //v = 1.0 - abs(v * 2.0 - 1.0);
  //v = pow(v, 2.0 + sin((uTime * 0.2 + noiseY * 0.25) * TAU) * 0.5); // pulse
 // v = pow(v, TAU);

  return v;
}

void main(){
  vec4 color;
  vec2 uv = gl_FragCoord.xy / uRes.xy;
  
  //float v = distY( uv.x  );
  uv -= ( 1.0 -  (2.0 * uBarWidth_1) + uBarDistance ); // so that bars' initial pos is left of the screen
    
  float scanSpeed; 
  if(uMoveUV)
  {
	scanSpeed = sin(uTime * 0.25) ;
	scanSpeed = (scanSpeed + 1.0 ) / 2.0; // so that sine values are between 0 and 1
	
	scanSpeed = map(scanSpeed, 0.0, 1.0, 0.05, 0.95);
			
  }
  else{
	scanSpeed = 0.0;
  }
  uv += scanSpeed ;

  if ( uv.x > (uRectPosX_1) && uv.x < ( uRectPosX_1 + uBarWidth_1) ){
    color = vec4( color1, 1.0);
  }
  else if ( uv.x > uRectPosX_2 && uv.x < ( uRectPosX_2 + uBarWidth_2 ) ) {
    color = vec4( color2, 1.0);
  }
  else if ( uv.x > uRespBarPosX && uv.x < ( uRespBarPosX + uRespBarWidth ) ){
      if(uDrawResp){
		color = vec4(color3, 1.0);    
	  }
  }
    
  gl_FragColor = color;
}
