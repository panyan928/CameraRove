/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglx_default_fonts.c
 DESCRIPTION       : Default font used by OGLX, this is a single square defined 
					as a stroke font
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

/* Default font structure */
typedef struct {
    GLuint ui_fonts_size;
    GLuint ui_fonts_number;
    GLuint ui_textures_number;
    GLuint ui_textures_offset;
    glf_type_font tr_fonts_description[1];
    glf_type_character_stroke characters[1];
    GLuint ui_nb_lines;
    GLushort us_line_type;
    GLushort us_nb_vertex;
    GLfloat tr_points[8];
} oglx_type_default_font;

/*+-----------------------------------------------------------------
  DESCRIPTION:
    Default font definition. This is a simple square character.
---------------------------------------------------------------------+*/
static oglx_type_default_font oglx_default_font = {
    /* Fonts size */
    sizeof(oglx_type_default_font),
    /* Fonts number */
    1U,
    /* Textures number */
    0U,
    /* Textures offset */
    0U,
    /* FONT DESCRIPTION */
    {
     {
      /* Font name */
      "Default font",
      /* Char size */
      sizeof(glf_type_character_stroke),
      /* Number of chars */
      1U,
      /* Font type */
      GLF_FONT_TYPE_STROKE,
      /* Line widths availability */
      {GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE}
      ,
      /* Line widths delta */
      {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}
      ,
      /* Halo line widths delta */
      {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}
      ,
      /* Padding */
      {0U}
      ,
      /* Underline and stroke */
      -3.433156F, 5.248456F,
      /* Maximum char width and height */
      7.350779F, 11.324512F,
      /* Maximum horizontal and vertical advance */
      10.756639F, 3.397354F,
      /* Offset to chars description */
      (sizeof(glf_type_font)),
      /* Offset to vectorial descriptions */
      (sizeof(glf_type_font) + sizeof(glf_type_character_stroke)),
      /* Delta to chars description */
      {0U}
      }
     }
    ,
    /* CHARS DESCRIPTION */
    {
     /* Char 0 */
     {{0U, 7.350779F, 11.324512F, 10.756639F, 3.397354F}
      , 0U}
     }
    ,
    /* VECTORIAL DESCRIPTIONS */
    /* Char 0 */
    1U,
    GLF_LINE_LOOP,
    4U,
    /* Default character is a rectangle */
    {
     0.368000F, 0.114000F,
     6.983000F, 0.114000F,
     6.983000F, 11.210000F,
     0.368000F, 11.210000F,
     }
};

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: getDefaultFonts
  DESCRIPTION:
    Function shall return the default font.
  SCOPE: global
  PARAMETERS:
    None
  RETURN:
    oglx_type_default_fonts -> the default font.
---------------------------------------------------------------------+*/
glf_type_font *getDefaultFont(void)
{
    return &(oglx_default_font.tr_fonts_description[0]);
}

/* End of File ***************************************************************/
