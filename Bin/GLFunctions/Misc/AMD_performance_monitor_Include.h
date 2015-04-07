#define GLI_INCLUDE_GL_AMD_PERFORMANCE_MONITOR

enum Main {

  GL_COUNTER_TYPE_AMD                   = 0x8BC0,
  GL_COUNTER_RANGE_AMD                  = 0x8BC1,
  GL_UNSIGNED_INT64_AMD                 = 0x8BC2,
  GL_PERCENTAGE_AMD                     = 0x8BC3,
  GL_PERFMON_RESULT_AVAILABLE_AMD       = 0x8BC4,
  GL_PERFMON_RESULT_SIZE_AMD            = 0x8BC5,
  GL_PERFMON_RESULT_AMD                 = 0x8BC6,

};

void glGetPerfMonitorGroupsAMD(GLint * numGroups, GLsizei groupsSize, GLuint * groups);
void glGetPerfMonitorCountersAMD(GLuint group, GLint * numCounters, GLint * maxActiveCounters, GLsizei counterSize, GLuint * counters);
void glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString);
void glGetPerfMonitorCounterStringAMD(GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length, GLchar * counterString);
void glGetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum[Main] pname, GLvoid * data);
void glGenPerfMonitorsAMD(GLsizei n, GLuint * monitors);
void glDeletePerfMonitorsAMD(GLsizei n, GLuint * monitors);
void glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint * counterList);
void glBeginPerfMonitorAMD(GLuint monitor);
void glEndPerfMonitorAMD(GLuint monitor);
void glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum[Main] pname, GLsizei dataSize, GLuint * data, GLint * bytesWritten);
