#define GLI_INCLUDE_GL_ARB_COMPUTE_VARIABLE_GROUP_SIZE

enum Main {

  GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB       = 0x9344,
  //GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB          = 0x90EB,
  GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB              = 0x9345,
  //GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB                 = 0x91BF,

};

void glDispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
