#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "clax.h"

  unsigned __int16 flare_count=0;
  unsigned __int16 flare_frame_count=0;
  unsigned long ulTemp;

  __int16		x_flare[1000000];
  __int16		y_flare[1000000];
  float		z_flare[1000000];
  unsigned __int16		id_flare[1000000];

void WriteString( char* str, FILE* out )
{
	char		null = 0;

	if( !str )
		fwrite( &null, sizeof(char), 1, out );
	else
		fwrite( str, sizeof(char), strlen(str) + 1, out );
}


void main ()
{
  c_SCENE  *scene;
  c_OBJECT *obj;
  w_NODE   *node;
  k_NODE   *kn;
  c_LIGHT  *lit;
  c_CAMERA *cam;
  c_VECTOR  vec;
  c_OBJECT *o1;
  char     *myscreen;
  __int16       err;
  __int16       i, x, y;
  float		frames, frame;
  

	FILE	*pFile;	// tu zapisujemy flarki

  unsigned __int16	ulFrameCount;


  pFile=fopen("flares.bin","wb");


  clax_init (clax_transform | clax_hierarchy | clax_domorph);
  clax_alloc_scene (&scene);
  err = clax_load_world ("scene.3ds",scene);
  if (err != clax_err_ok)
    printf("error(after):%s\n",clax_geterror(err));
      else printf("ok!\n");

  err = clax_load_motion ("scene.3ds",scene);
  if (err != clax_err_ok)
    printf("error(after): %s\n",clax_geterror(err));
      else printf("ok!\n");
  clax_setactive_scene (scene);

  clax_byname ("Camera01", &node);
  if (node) cam = (c_CAMERA *)node->object; else {
    clax_byname("rolli",&node);
    cam = (c_CAMERA *)node->object;
  }
  cam->sizeX = 1024.0;
  cam->sizeY = 768.0;
  cam->aspectratio = 0.75;

  clax_getframes(&frame, &frames);
  clax_setactive_camera (cam);

  WriteString("FOX3D",pFile);
  
	ulFrameCount=(unsigned __int16)frames;
	fwrite(&ulFrameCount,1,sizeof(unsigned __int16), pFile);

  
  for (frame=0;frame<frames;frame += 1) {

	ulFrameCount=(unsigned __int16)frame;
	fwrite(&ulFrameCount,1,sizeof(unsigned __int16),pFile);


/********************************
- numer klatki
- ile flar
- flara - x, y, z, id
*********************************/

	clax_setframe(frame);
    clax_update();

	// clearscreen

    for (node = scene->world; node; node=node->next) {
      if (node->type == clax_obj_object) {
        obj = (c_OBJECT *)node->object;
        if (((obj->flags & clax_obj_hidden) == 0)&&
             (obj->flags & clax_obj_chidden) == 0)
        for (i=0;i<obj->numverts;i++) {
//          mat_mulvec (cam->matrix, &obj->vertices[i].pvert, &vec);
          vec_copy(&obj->vertices[i].pvert, &vec);
          x = 512+vec.x*cam->perspX/vec.z;
          y = 384-vec.y*cam->perspY/vec.z;
          if (x>=-256 && x<1280 && y>=-256 && y<1024 && vec.z > 1.0) 
		  {
			x_flare[flare_frame_count] = x;
			y_flare[flare_frame_count] = y;
			z_flare[flare_frame_count] = vec.z;
			id_flare[flare_frame_count] = flare_count;
			
			flare_frame_count++;
		  }
			flare_count++;
			  //myscreen[y*320+x] = 15;
        }
      } else if (node->type == clax_obj_light) {
        lit = (c_LIGHT *)node->object;
        mat_mulvec (cam->matrix, &lit->pos, &vec);
        x = 512+vec.x*cam->perspX/vec.z;
        y = 384-vec.y*cam->perspY/vec.z;
        if (x>=0 && x<800 && y>=0 && y<600 && vec.z > 1.0) {
			printf("LIGHT AT frame : %l x : %l y: %l z: %f no: %d\n",frame,x,y,vec.z,flare_count);


        }

      }
    }
    // flip

	fwrite(&flare_frame_count,1,sizeof(unsigned __int16),pFile); // ile flar ?

	for (ulTemp=0; ulTemp<flare_frame_count; ulTemp++)  // dane flar
	{	
		fwrite(&x_flare[ulTemp],1, sizeof(__int16), pFile);
		fwrite(&y_flare[ulTemp],1, sizeof(__int16), pFile);
		fwrite(&z_flare[ulTemp],1, sizeof(float), pFile);
		fwrite(&id_flare[ulTemp],1, sizeof(unsigned __int16), pFile);
	}
	
	flare_count=0;
	flare_frame_count=0;

  }

  printf("freeing world\n");
 // clax_free_world(scene);
  printf("freeing motion\n");
  clax_free_motion(scene);
  printf("OK!\n");
  fclose(pFile);

}

