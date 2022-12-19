#version 330 core

in vec3 n;
in vec3 e;
in vec3 l;
in vec2 TexCoord;
out vec4 outColor;

uniform sampler2D texture1;

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.3, 0.4);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

float spec_exp = 5;

//ambient
vec3 Ia = ka*La;

//diffuse
vec3 Id = kd*max(dot(n, l)*Ld, 0.0);

//specular

vec3 v = normalize(e);
vec3 r = normalize(reflect(-l,n));
vec3 h = normalize(l+v);
vec3 Is = ks*Ls*max(pow(dot(h, n), spec_exp),0);

vec3 fColor = Ia + Id +Is;

//oclusion map

//float lay_currd=0.0f;

//float lay_min=10.0f;
//float lay_max=60.0f;
//float lay_n=mix(lay_max,lay_min, abs(dot(vec3(0.0f,0.0f,1.0f),v)));
//float h_scale=0.05f;
//float lay_depth=1.0f/lay_n;

//float curr_d_map=1.0f-texture(displacement0,TexCoord).r;
//vec2 t_texcoord=v.xy/v.z*h_scale;

//while(lay_currd<curr_d_map){
  //      curr_d_map=1.0f-texture(displacement,t_texcoord).r;
  //      curr_d_map+=lay_depth;
  //      lay_currd+=lay_depth
//}
//float after_d=curr_d_map-lay_currd;
//vec2 prev_texcoord=TexCoord+t_texcoord;

//float before_d=1.0f-texture(displacement0,prev_texcoord).r - lay_currd+lay_depth;
//weight = after/(after-before);
//TexCoord=prev_texcoord*wgt + TexCoord*(1.0f - wgt);

//if(TexCoord.x>1.0 || TexCoord.y>1.0 || TexCoord.x<0.0 || TexCoord.y<0.0)
  //      discard;

//n=normalize(texture(n, TexCoord).xyz * 2.0f -1.0f);



void main(void) {
        outColor = 0.35*vec4(fColor, 1.0) + 0.65*texture(texture1, TexCoord);
}
