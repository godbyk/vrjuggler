#ifndef GL_DRAW_WAND_FUNCTORS_H
#define GL_DRAW_WAND_FUNCTORS_H

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif


namespace vrj
{

/** Draw a basic cone wand
*/ 
class GlDrawConeWandFunctor : public GlDrawObjectFunctor
{
public:
   /* Called to initialize any context specific information
   */
   virtual void contextInit()
   {;}
   
   /** Callback function for drawing
   * Called when the object should be drawn
   * @pre GL context is set and ready to go
   */
   virtual void draw()
   {
      const float base = 0.2;
      const float height = 0.6f;
      const int slices = 6;
      const int stacks = 1;
   
      if (mQuadObj == NULL)
         mQuadObj = gluNewQuadric();

      glColor3f(0.0f, 1.0f, 0.0f);
      gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
      gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
      gluCylinder(mQuadObj, base, 0.0, height, slices, stacks);
   }

protected:
   GLUquadricObj* mQuadObj;
};


/** Draw a wand with a right angle
*/ 
class GlDrawRightAngleWandFunctor : public GlDrawObjectFunctor
{
public:
   /* Called to initialize any context specific information
   */
   virtual void contextInit()
   {;}
   
   /** Callback function for drawing
   * Called when the object should be drawn
   * @pre GL context is set and ready to go
   */
   virtual void draw()
   {
      static GLfloat VertexData[] = {
   		0.0500000f, -0.500000f, 0.0500000f, 0.0500000f, 0.100000f, 0.0500000f, -0.0500000f, 0.100000f, 0.0500000f, 
   		-0.0500000f, -0.500000f, 0.0500000f, 0.0500000f, -0.500000f, -0.0500000f, 
   		0.0500000f, 0.00000f, -0.0500000f, 0.0500000f, 0.100000f, 0.0500000f, 
   		0.0500000f, -0.500000f, 0.0500000f, -0.0500000f, -0.500000f, -0.0500000f, 
   		-0.0500000f, 0.00000f, -0.0500000f, 0.0500000f, 0.00000f, -0.0500000f, 
   		0.0500000f, -0.500000f, -0.0500000f, -0.0500000f, -0.500000f, 0.0500000f, 
   		-0.0500000f, 0.100000f, 0.0500000f, -0.0500000f, 0.00000f, -0.0500000f, 
   		-0.0500000f, -0.500000f, -0.0500000f, -0.0500000f, -0.500000f, -0.0500000f, 
   		0.0500000f, -0.500000f, -0.0500000f, 0.0500000f, -0.500000f, 0.0500000f, 
   		-0.0500000f, -0.500000f, 0.0500000f, -0.0500000f, 0.100000f, 0.0500000f, 
   		0.0500000f, 0.100000f, 0.0500000f, 0.100000f, 0.200000f, -0.150000f, 
   		-0.100000f, 0.200000f, -0.150000f, 0.100000f, 0.150000f, -0.150000f, 
   		-0.100000f, 0.150000f, -0.150000f, -0.100000f, 0.200000f, -0.150000f, 
   		0.100000f, 0.200000f, -0.150000f, 0.0500000f, 0.100000f, 0.0500000f, 
   		0.0500000f, 0.00000f, -0.0500000f, 0.100000f, 0.150000f, -0.150000f, 
   		0.0500000f, 0.100000f, 0.0500000f, 0.100000f, 0.150000f, -0.150000f, 
   		0.100000f, 0.200000f, -0.150000f, -0.0500000f, 0.100000f, 0.0500000f, 
   		-0.100000f, 0.200000f, -0.150000f, -0.100000f, 0.150000f, -0.150000f, 
   		-0.100000f, 0.150000f, -0.150000f, -0.0500000f, 0.00000f, -0.0500000f, 
   		-0.0500000f, 0.100000f, 0.0500000f, 0.100000f, 0.150000f, -0.150000f, 
   		0.0500000f, 0.00000f, -0.0500000f, -0.0500000f, 0.00000f, -0.0500000f, 
   		-0.100000f, 0.150000f, -0.150000f
   	};
   	static GLint Indices[] = {
   		0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 
   		10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 
   		22, 20, 22, 23, 24, 25, 26, 24, 26, 27, 28, 29, 30, 31, 32, 33, 
   		34, 35, 36, 37, 38, 39, 40, 41, 42, 40, 42, 43
   	};
   
   	glEnableClientState(GL_VERTEX_ARRAY);
   	glVertexPointer(3, GL_FLOAT, 0, VertexData);
   
   	glColor3f(0.00000f, 1.00000f, 0.00000f);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[0]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[3]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[6]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[9]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[12]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[15]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[18]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[21]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[24]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[27]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[30]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[33]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[36]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[39]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[42]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[45]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[48]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[51]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[54]);
   	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &Indices[57]);
   }

};


};

#endif


