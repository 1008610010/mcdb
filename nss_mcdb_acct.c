#include "nss_mcdb_acct.h"
#include "nss_mcdb.h"
#include "mcdb.h"
#include "uint32.h"
#include "code_attributes.h"

#include <errno.h>
#include <string.h>

#include <pwd.h>
#include <grp.h>
#include <shadow.h>

/*
 * man passwd(5) getpwnam getpwuid setpwent getpwent endpwent
 *     /etc/passwd
 * man group(5)  getgrnam getgrgid setgrent getgrent endgrent
 *     /etc/group
 * man shadow(5) getspnam
 *     /etc/shadow
 */

static enum nss_status
_nss_mcdb_decode_passwd(struct mcdb * restrict,
                        const struct _nss_kinfo * restrict __attribute_unused__,
                        const struct _nss_vinfo * restrict)
  __attribute_nonnull_x__((1,3))  __attribute_warn_unused_result__;

static enum nss_status
_nss_mcdb_decode_group(struct mcdb * restrict,
                       const struct _nss_kinfo * restrict __attribute_unused__,
                       const struct _nss_vinfo * restrict)
  __attribute_nonnull_x__((1,3))  __attribute_warn_unused_result__;

static enum nss_status
_nss_mcdb_decode_spwd(struct mcdb * restrict,
                      const struct _nss_kinfo * restrict __attribute_unused__,
                      const struct _nss_vinfo * restrict)
  __attribute_nonnull_x__((1,3))  __attribute_warn_unused_result__;


void _nss_files_setpwent(void) { _nss_mcdb_setent(NSS_DBTYPE_PASSWD); }
void _nss_files_endpwent(void) { _nss_mcdb_endent(NSS_DBTYPE_PASSWD); }
void _nss_files_setgrent(void) { _nss_mcdb_setent(NSS_DBTYPE_GROUP);  }
void _nss_files_endgrent(void) { _nss_mcdb_endent(NSS_DBTYPE_GROUP);  }
void _nss_files_setspent(void) { _nss_mcdb_setent(NSS_DBTYPE_SHADOW); }
void _nss_files_endspent(void) { _nss_mcdb_endent(NSS_DBTYPE_SHADOW); }


enum nss_status
_nss_files_getpwent_r(struct passwd * const restrict pwbuf,
                      char * const restrict buf, const size_t buflen,
                      struct passwd ** const restrict pwbufp)
{
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_passwd,
                                      .vstruct = pwbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= pwbufp };
    *pwbufp = NULL;
    return _nss_mcdb_getent(NSS_DBTYPE_PASSWD, &vinfo);
}

enum nss_status
_nss_files_getpwnam_r(const char * const restrict name,
                      struct passwd * const restrict pwbuf,
                      char * const restrict buf, const size_t buflen,
                      struct passwd ** const restrict pwbufp)
{
    const struct _nss_kinfo kinfo = { .key     = name,
                                      .klen    = strlen(name),
                                      .tagc    = (unsigned char)'=' };
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_passwd,
                                      .vstruct = pwbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= pwbufp };
    *pwbufp = NULL;
    return _nss_mcdb_get_generic(NSS_DBTYPE_PASSWD, &kinfo, &vinfo);
}

enum nss_status
_nss_files_getpwuid_r(const uid_t uid,
                      struct passwd * const restrict pwbuf,
                      char * const restrict buf, const size_t buflen,
                      struct passwd ** const restrict pwbufp)
{
    char hexstr[8];
    const struct _nss_kinfo kinfo = { .key     = hexstr,
                                      .klen    = sizeof(hexstr),
                                      .tagc    = (unsigned char)'x' };
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_passwd,
                                      .vstruct = pwbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= pwbufp };
    *pwbufp = NULL;
    uint32_to_ascii8uphex((uint32_t)uid, hexstr);
    return _nss_mcdb_get_generic(NSS_DBTYPE_PASSWD, &kinfo, &vinfo);
}


enum nss_status
_nss_files_getgrent_r(struct group * const restrict grbuf,
                      char * const restrict buf, const size_t buflen,
                      struct group ** const restrict grbufp)
{
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_group,
                                      .vstruct = grbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= grbufp };
    *grbufp = NULL;
    return _nss_mcdb_getent(NSS_DBTYPE_GROUP, &vinfo);
}

enum nss_status
_nss_files_getgrnam_r(const char * const restrict name,
                      struct group * const restrict grbuf,
                      char * const restrict buf, const size_t buflen,
                      struct group ** const restrict grbufp)
{
    const struct _nss_kinfo kinfo = { .key     = name,
                                      .klen    = strlen(name),
                                      .tagc    = (unsigned char)'=' };
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_group,
                                      .vstruct = grbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= grbufp };
    *grbufp = NULL;
    return _nss_mcdb_get_generic(NSS_DBTYPE_GROUP, &kinfo, &vinfo);
}

enum nss_status
_nss_files_getgrgid_r(const gid_t gid,
                      struct group * const restrict grbuf,
                      char * const restrict buf, const size_t buflen,
                      struct group ** const restrict grbufp)
{
    char hexstr[8];
    const struct _nss_kinfo kinfo = { .key     = hexstr,
                                      .klen    = sizeof(hexstr),
                                      .tagc    = (unsigned char)'x' };
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_group,
                                      .vstruct = grbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= grbufp };
    *grbufp = NULL;
    uint32_to_ascii8uphex((uint32_t)gid, hexstr);
    return _nss_mcdb_get_generic(NSS_DBTYPE_GROUP, &kinfo, &vinfo);
}


enum nss_status
_nss_files_getspent_r(struct spwd * const restrict spbuf,
                      char * const restrict buf, const size_t buflen,
                      struct spwd ** const restrict spbufp)
{
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_spwd,
                                      .vstruct = spbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= spbufp };
    *spbufp = NULL;
    return _nss_mcdb_getent(NSS_DBTYPE_SHADOW, &vinfo);
}

enum nss_status
_nss_files_getspnam_r(const char * const restrict name,
                      struct spwd * const restrict spbuf,
                      char * const restrict buf, const size_t buflen,
                      struct spwd ** const restrict spbufp)
{
    const struct _nss_kinfo kinfo = { .key     = name,
                                      .klen    = strlen(name),
                                      .tagc    = (unsigned char)'=' };
    const struct _nss_vinfo vinfo = { .decode  = _nss_mcdb_decode_spwd,
                                      .vstruct = spbuf,
                                      .buf     = buf,
                                      .buflen  = buflen,
                                      .vstructp= spbufp };
    *spbufp = NULL;
    return _nss_mcdb_get_generic(NSS_DBTYPE_SHADOW, &kinfo, &vinfo);
}



/* Consider: for last few ounces of performance -- at the cost of making it
 * difficult for human to read record dumped from database with mcdb tools --
 * store numbers in defined byte-order, instead of converting to ASCII hex
 * and back.  Also, store '\0' in data instead of replacing after memcpy() */


static enum nss_status
_nss_mcdb_decode_passwd(struct mcdb * const restrict m,
                        const struct _nss_kinfo * restrict kinfo,
                        const struct _nss_vinfo * restrict vinfo)
{
    char * const restrict dptr = (char *)mcdb_dataptr(m);
    const size_t dlen = ((size_t)mcdb_datalen(m)) - IDX_PW_HDRSZ;
    struct passwd * const pw = (struct passwd *)vinfo->vstruct;
    char * const buf = vinfo->buf;
    /* parse fixed format record header to get offsets into strings */
    const uintptr_t idx_pw_passwd = uint16_from_ascii4uphex(dptr+IDX_PW_PASSWD);
    const uintptr_t idx_pw_gecos  = uint16_from_ascii4uphex(dptr+IDX_PW_GECOS);
    const uintptr_t idx_pw_dir    = uint16_from_ascii4uphex(dptr+IDX_PW_DIR);
    const uintptr_t idx_pw_shell  = uint16_from_ascii4uphex(dptr+IDX_PW_SHELL);
    pw->pw_uid   = (uid_t)          uint32_from_ascii8uphex(dptr+IDX_PW_UID);
    pw->pw_gid   = (gid_t)          uint32_from_ascii8uphex(dptr+IDX_PW_GID);
    /* populate pw string pointers */
    pw->pw_name  = buf;
    pw->pw_passwd= buf+idx_pw_passwd;
    pw->pw_gecos = buf+idx_pw_gecos;
    pw->pw_dir   = buf+idx_pw_dir;
    pw->pw_shell = buf+idx_pw_shell;
    if (dlen < vinfo->buflen) {
        *((struct passwd **)vinfo->vstructp) = pw;
        memcpy(buf, dptr+IDX_PW_HDRSZ, dlen);
        /* terminate strings; replace ':' separator in data with '\0' */
        buf[idx_pw_passwd-1] = '\0';     /* terminate pw_name   */
        buf[idx_pw_gecos-1]  = '\0';     /* terminate pw_passwd */
        buf[idx_pw_dir-1]    = '\0';     /* terminate pw_gecos  */
        buf[idx_pw_shell-1]  = '\0';     /* terminate pw_dir    */
        buf[dlen]            = '\0';     /* terminate pw_shell  */
        return NSS_STATUS_SUCCESS;
    }
    else {
        *((struct passwd **)vinfo->vstructp) = NULL;
        errno = ERANGE;
        return NSS_STATUS_TRYAGAIN;
    }
}


static enum nss_status
_nss_mcdb_decode_group(struct mcdb * const restrict m,
                       const struct _nss_kinfo * const restrict kinfo,
                       const struct _nss_vinfo * const restrict vinfo)
{
    char * const restrict dptr = (char *)mcdb_dataptr(m);
    struct group * const gr = (struct group *)vinfo->vstruct;
    char *buf = vinfo->buf;
    /* parse fixed format record header to get offsets into strings */
    const uintptr_t idx_gr_passwd =uint16_from_ascii4uphex(dptr+IDX_GR_PASSWD);
    const uintptr_t idx_gr_mem_str=uint16_from_ascii4uphex(dptr+IDX_GR_MEM_STR);
    const uintptr_t idx_gr_mem    =uint16_from_ascii4uphex(dptr+IDX_GR_MEM);
    const size_t gr_mem_num       =uint16_from_ascii4uphex(dptr+IDX_GR_MEM_NUM);
    char ** const restrict gr_mem =
      (char **)(((uintptr_t)(buf+idx_gr_mem+0x7u)) & ~0x7u); /* 8-byte align */
    gr->gr_mem   = gr_mem;
    gr->gr_gid   = (gid_t)         uint32_from_ascii8uphex(dptr+IDX_GR_GID);
    /* populate gr string pointers */
    gr->gr_name  = buf;
    gr->gr_passwd= buf+idx_gr_passwd;
    /* fill buf, (char **) gr_mem (allow 8-byte ptrs), and terminate strings.
     * scan for ',' instead of precalculating array because names should
     * be short and adding an extra 4 chars per name to store size takes
     * more space and might take just as long to parse as scan for ','
     * (assume data consistent, gr_mem_num correct) */
    if (((char *)gr_mem)-buf+((gr_mem_num+1)<<3) <= vinfo->buflen) {
        *((struct group **)vinfo->vstructp) = gr;
        memcpy(buf, dptr+IDX_GR_HDRSZ, mcdb_datalen(m)-IDX_GR_HDRSZ);
        /* terminate strings; replace ':' separator in data with '\0'. */
        buf[idx_gr_passwd-1]  = '\0';        /* terminate gr_name   */
        buf[idx_gr_mem_str-1] = '\0';        /* terminate gr_passwd */
        buf[idx_gr_mem-1]     = '\0';        /* terminate final gr_mem string */
        gr_mem[0] = (buf += idx_gr_mem_str); /* begin of gr_mem strings */
        for (size_t i=1; i<gr_mem_num; ++i) {/*(i=1; assigned first str above)*/
            while (*buf != ',')   /*(*++buf if no 0-len names check in create)*/
                ++buf;
            *buf++ = '\0';
            gr_mem[i] = buf;
        }
        gr_mem[gr_mem_num] = NULL;         /* terminate (char **) gr_mem array*/
        return NSS_STATUS_SUCCESS;
    }
    else {
        *((struct group **)vinfo->vstructp) = NULL;
        errno = ERANGE;
        return NSS_STATUS_TRYAGAIN;
    }
}


static enum nss_status
_nss_mcdb_decode_spwd(struct mcdb * const restrict m,
                      const struct _nss_kinfo * const restrict kinfo,
                      const struct _nss_vinfo * const restrict vinfo)
{
    char * const restrict dptr = (char *)mcdb_dataptr(m);
    const size_t dlen = ((size_t)mcdb_datalen(m)) - IDX_SP_HDRSZ;
    struct spwd * const sp = (struct spwd *)vinfo->vstruct;
    char * const buf = vinfo->buf;
    /* parse fixed format record header to get offsets into strings */
    const uintptr_t idx_sp_pwdp = uint16_from_ascii4uphex(dptr+IDX_SP_PWDP);
    sp->sp_lstchg    = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_LSTCHG);
    sp->sp_min       = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_MIN);
    sp->sp_max       = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_MAX);
    sp->sp_warn      = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_WARN);
    sp->sp_inact     = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_INACT);
    sp->sp_expire    = (long int) uint32_from_ascii8uphex(dptr+IDX_SP_EXPIRE);
    sp->sp_flag      =            uint32_from_ascii8uphex(dptr+IDX_SP_FLAG);
    /* populate sp string pointers */
    sp->sp_namp  = buf;
    sp->sp_pwdp  = buf+idx_sp_pwdp;
    if (dlen < vinfo->buflen) {
        *((struct spwd **)vinfo->vstructp) = sp;
        memcpy(buf, dptr+IDX_SP_HDRSZ, dlen);
        /* terminate strings; replace ':' separator in data with '\0' */
        buf[idx_sp_pwdp-1] = '\0';     /* terminate sp_namp */
        buf[dlen]          = '\0';     /* terminate sp_pwdp */
        return NSS_STATUS_SUCCESS;
    }
    else {
        *((struct spwd **)vinfo->vstructp) = NULL;
        errno = ERANGE;
        return NSS_STATUS_TRYAGAIN;
    }
}