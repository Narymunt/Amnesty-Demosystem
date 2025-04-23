#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "clax.h"

  unsigned long flare_count=0;
  unsigned long flare_frame_count=0;
  unsigned long ulTemp;

  int		x_flare[100000];
  int		y_flare[100000];
  float		z_flare[100000];
  unsigned long		id_flare[100000];


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
  int       err;
  int       i, x, y;
  float		frames, frame;
  

	FILE	*pFile;	// tu zapisujemy flarki

  unsigned long	ulFrameCount;


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
  cam->sizeX = 800.0;
  cam->sizeY = 600.0;
  cam->aspectratio = 0.75;

  clax_getframes(&frame, &frames);
  clax_setactive_camera (cam);

	ulFrameCount=(unsigned long)frames;
	fwrite(&ulFrameCount,1,sizeof(unsigned long), pFile);

  
  for (frame=0;frame<frames;frame += 1) {

	printf("frame #: %f\n",frame);

	ulFrameCount=(unsigned long)frame;
	fwrite(&ulFrameCount,1,sizeof(unsigned long),pFile);


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
          x = 400+vec.x*cam->perspX/vec.z;
          y = 300-vec.y*cam->perspY/vec.z;
          if (x>=0 && x<800 && y>=0 && y<600 && vec.z > 1.0) 
		  {
			x_flare[flare_frame_count] = x;
			y_flare[flare_frame_count] = y;
			z_flare[flare_frame_count] = vec.z;
			id_flare[flare_frame_count] = flare_count;
			
			flare_frame_count++;
		  }
			flare_count++;
			  //		printf("POINT AT frame : %d x : %d y: %d z: %d\n",frame,x,y,vec.z);
			  //myscreen[y*320+x] = 15;
        }
      } else if (node->type == clax_obj_light) {
        lit = (c_LIGHT *)node->object;
        mat_mulvec (cam->matrix, &lit->pos, &vec);
        x = 400+vec.x*cam->perspX/vec.z;
        y = 300-vec.y*cam->perspY/vec.z;
        if (x>=0 && x<800 && y>=0 && y<600 && vec.z > 1.0) {
			printf("LIGHT AT frame : %l x : %l y: %l z: %f no: %d\n",frame,x,y,vec.z,flare_count);


        }

      }
    }
    // flip

	fwrite(&flare_frame_count,1,sizeof(unsigned long),pFile); // ile flar ?

	for (ulTemp=0; ulTemp<flare_frame_count; ulTemp++)  // dane flar
	{	
		fwrite(&x_flare[ulTemp],1, sizeof(int), pFile);
		fwrite(&y_flare[ulTemp],1, sizeof(int), pFile);
		fwrite(&z_flare[ulTemp],1, sizeof(float), pFile);
		fwrite(&id_flare[ulTemp],1, sizeof(unsigned long), pFile);
	}

	flare_count=0;
	flare_frame_count=0;

  }

  printf("freeing world\n");
  clax_free_world(scene);
  printf("freeing motion\n");
  clax_free_motion(scene);
  printf("OK!\n");
  fclose(pFile);

}

