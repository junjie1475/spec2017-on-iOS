#define NAME_WITH_PREFIX_AND_REVISION(prefix, revision, name) \
  prefix##revision##name
#define RTNAME(name) NAME_WITH_PREFIX_AND_REVISION(_Fortran, A, name)
struct EnvironmentDefaultList;
void RTNAME(ProgramStart)(
    int, const char *[], const char *[], const struct EnvironmentDefaultList *);
void RTNAME(ByteswapOption)(void); // -byteswapio
/* main entry into PROGRAM */
void _QQmain_wrf(void);

extern const struct EnvironmentDefaultList *_QQEnvironmentDefaults;

/* C main stub */
int wrf_entry(int argc, const char *argv[], const char *envp[]) {
  RTNAME(ProgramStart)(argc, argv, envp, _QQEnvironmentDefaults);
  _QQmain_wrf();
  return 0;
}
