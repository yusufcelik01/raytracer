# Ray Tracer
This is a raytracer that I am writing from scratch for the course ceng795 at metu ceng. It is basically what a core path tracer should be for general applications.

![glass_next_importance_sample16384](https://user-images.githubusercontent.com/47708508/226272492-21712a3e-b581-4d20-aef6-3504903410d9.png)

![witcher_medallion_glacier_perlin](https://user-images.githubusercontent.com/47708508/226272986-a46338ac-8510-45e7-9bcf-28598ec360c9.png)

# Features
- Basic linear transformations and Mesh Instancing
- Bounding Volume Hierarchy
- Multisampling
  - Motion Blurr
  - Depth of Field
  - Rough/Glossy surfaces
- Textures
  - Diffuse/Specular Maps
  - Normal Maps
  - Bump maps
  - Perlin Noise
  - Checker Board Procedural Textures
- BRDFs 
  - Blinn-Phong
  - Phong 
  - Torrance-Sparrow
- HDR rendering and Reinhard tonemapping
- Path Tracing
  - Importance Sampling
  - Next Event Estimation
  - Russian Roulette

# Supported Light sources
- Point Light
- Area Light
- Spot Light 
- Directional Light
- HDR Environment Maps
- Object Lights

## More Sample Outputs

![scienceTree_glass](https://user-images.githubusercontent.com/47708508/226274775-9bb5b4d7-1186-466e-92aa-d60ced018bd0.png)

![witcher_medallion_grace_perlin](https://user-images.githubusercontent.com/47708508/226273125-b7dd135a-0e0b-4c7c-92ad-da0e46bd7eb8.png)

![cornellbox_motion_area](https://user-images.githubusercontent.com/47708508/226271374-7d7a5f38-e4fd-4681-a348-6fb113d9a7b6.png)

![dragon_color_spot_lights_hdr](https://user-images.githubusercontent.com/47708508/226271487-a4621384-8723-4cf2-83e0-5723d4b8943b.png)

![wood_box_all](https://user-images.githubusercontent.com/47708508/226274361-b640869d-bf46-41da-afc9-92a34491e00c.png)




