/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*     		1999 - 2010, All Rights Reserved		        */
/*									*/
/* MODULE NAME : sx_dec.c						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 02/22/12  JRB	   Fix Klocwork warnings.			*/
/* 01/31/12  JRB	   sx_rip_xml: use isspace to skip over white space.*/
/* 11/23/11  KCR           Added casting for proper 64bit compile       */
/* 07/05/11  JRB	   For expat, call XML_StopParser if		*/
/*			   sxStartElememt or sxStopElement fails.	*/
/* 04/01/11  NAV           Add back sx_parseEx_mt to buil AX-S4 ICCP	*/
/* 08/02/10  JRB	   Close file if fstat fails.			*/
/* 05/28/10  JRB	   sx_get_*: use asciiTo* for better err checking*/
/*			   sx_get_*_attr: return 2 different errors.	*/
/* 03/26/10  JRB	   sx_get_time_ex,sx_get_tm_ex: fix microsec conv*/
/* 03/01/10  JRB	   Make sure xmlNestLevel value is legal.	*/
/* 02/26/10  JRB	   Del obsolete sx_parseEx_mt, eliminate useFp	*/
/*			   flag, simplify remaining code.		*/
/*			   Fix endTagBuf size.				*/
/* 02/06/08  EJV     88    Added SX_PARSING_OK, sx_err_str* (for slogs).*/
/* 01/14/08  MDE     87    Fixed numOcc multithread problem		*/
/* 10/04/07  MDE     86    Tweaked LOGCFG_VALUE_GROUP/LOGCFGX_VALUE_MAP	*/
/* For early MODLOGS see previous revision of this file on PVCS.	*/
/************************************************************************/

#if defined (_WIN32)
#pragma warning(disable : 4996)
#endif

#include "glbtypes.h"
#include "sysincs.h"
#include "mem_chk.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "time_str.h"
#include "str_util.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

#if defined(USE_EXPAT)
/* Assume linking to Expat "static" library. Must define XML_STATIC	*/
/* before including "expat.h".						*/
#define XML_STATIC
#include "expat.h"
#define EXPAT_BUF_SIZE	8192	/* parse buffer size if reading from file*/
#define ENTITY_BUF_SIZE	2000	/* initial size to alloc for entity buffer*/
				/* reallocate larger if needed		*/

static void XMLCALL expatHandlerStartSkip(void *userData, const char *el, const char **attr);
static void XMLCALL expatHandlerEndSkip(void *userData, const char *el);
#endif
/************************************************************************/

SD_CONST ST_CHAR *SD_CONST _sx_log_dec_logstr = "SX_LOG_DEC";
SD_CONST ST_CHAR *SD_CONST _sx_log_enc_logstr = "SX_LOG_ENC";
SD_CONST ST_CHAR *SD_CONST _sx_log_flow_logstr = "SX_LOG_FLOW";
SD_CONST ST_CHAR *SD_CONST _sx_log_debug_logstr = "SX_LOG_DEBUG";
SD_CONST ST_CHAR *SD_CONST _sx_log_err_logstr = "SX_LOG_ERR";
SD_CONST ST_CHAR *SD_CONST _sx_log_nerr_logstr = "SX_LOG_NERR";

ST_UINT sx_debug_sel = SX_LOG_ERR | SX_LOG_NERR;
ST_UINT sx_debug_sel_cfg = SX_LOG_ERR | SX_LOG_NERR;	/* saves SX masks while logging configuration is parsed */

#ifdef DEBUG_SISCO
LOGCFGX_VALUE_MAP sxLogMaskMaps[] =
  {
    {"SX_LOG_ERR",	SX_LOG_ERR,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Error"},
    {"SX_LOG_NERR",	SX_LOG_NERR,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Notice"},
    {"SX_LOG_FLOW",	SX_LOG_FLOW,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Flow"},
    {"SX_LOG_DEC",	SX_LOG_DEC,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Decode"},
    {"SX_LOG_ENC",	SX_LOG_ENC,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Encode"},
    {"SX_LOG_DEBUG",	SX_LOG_DEBUG,	&sx_debug_sel_cfg, _LOGCFG_DATATYPE_UINT_MASK, NULL, "Debug"},
  };

LOGCFG_VALUE_GROUP sxLogMaskMapCtrl =
  {
  {NULL,NULL},
  "SxLogMasks",	     /* Parent Tag */
  sizeof(sxLogMaskMaps)/sizeof(LOGCFGX_VALUE_MAP),
  sxLogMaskMaps
  };

/* tbl of error strings, used in slogs */
ST_CHAR *sx_err_str[] =
  {
  "SX_PARSING_OK",              /*  0 */
  "SX_USER_ERROR",              /*  1 */
  "SX_STRUCT_NOT_FOUND",        /*  2 */
  "SX_REQUIRED_TAG_NOT_FOUND",  /*  3 */
  "SX_DUPLICATE_NOT_ALLOWED",   /*  4 */
  "SX_EMPTY_TAG_NOT_ALLOWED",   /*  5 */
  "SX_XML_NEST_TOO_DEEP",       /*  6 */
  "SX_XML_BUFFER_OVER_MAX",     /*  7 */
  "SX_XML_MALFORMED",           /*  8 */
  "SX_FILE_NOT_FOUND",	        /*  9 */
  "SX_END_PARSING",             /* 10 */
  "SX_ELEMENT_TBL_TOO_BIG",     /* 11 */
  "SX_ERR_ATTR_NOT_FOUND",      /* 12 */
  "SX_ERR_REQUIRED_ATTR_NOT_FOUND", /* 13 */
  "SX_ERR_ALLOC"                /* 14 */
  };
ST_INT sx_err_str_cnt = sizeof (sx_err_str)/sizeof (ST_CHAR *);
#endif /* DEBUG_SISCO */



/************************************************************************/

ST_BOOLEAN sxUseSax;
ST_BOOLEAN sxIgnoreNS;

ST_RET sx_rip_xml (SX_DEC_CTRL *sxDecCtrl);
ST_RET sx_rip_xml_file (SX_DEC_CTRL *sxDecCtrl);
ST_RET sx_rip_xml_mem (SX_DEC_CTRL *sxDecCtrl);

SX_ELEMENT *_uibed_find_element (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag, ST_INT **numOccOut);
static ST_VOID _sx_pop (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN auto_pop);
ST_VOID str_to_microsec (ST_CHAR *str, ST_LONG *microseconds);

/************************************************************************/
/*				sx_parseExx_mt 				*/
/* Note: when making changes/correction to this function revise also	*/
/*       other sx_parse functions.					*/
/* Decode the XML located in file by loading the whole thing into	*/
/* memory first ...							*/
/************************************************************************/

ST_RET sx_parseExx_mt (ST_CHAR *fileName, ST_INT numItems, 
		   SX_ELEMENT *itemTbl, ST_VOID *usr,
                   ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                   ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
  {
ST_RET rc;
ST_LONG fileSize;
ST_LONG bytesRead;
ST_CHAR *cfgData;
struct stat buf;
int result;
FILE *fp;

/* Get the size of the file */
  fp = fopen (fileName, "r");
  if (fp == NULL)
    {
    SXLOG_FLOW2 ("XML File (%s) Open Error (errno=%d)", fileName, errno);
    return (SX_FILE_NOT_FOUND);
    }
  result = fstat (fileno (fp), &buf);
  if (result != 0)
    {
    fclose (fp);
    return (SD_FAILURE);
    }

/* Allocate a buffer and read all into memory */
  fileSize = buf.st_size;
  cfgData = (ST_CHAR *) chk_malloc (fileSize);

  bytesRead = (ST_LONG) fread (cfgData, 1, fileSize, fp);
  fclose (fp);
  if (bytesRead <= 0)
    {
    chk_free (cfgData);
    SXLOG_FLOW1 ("Error: Could not read from '%s'", fileName);
    return (SX_FILE_NOT_FOUND);
    }

  rc = sx_parse_mt (bytesRead, cfgData, numItems, itemTbl, usr, 
  		    u_sx_el_start_fun, u_sx_el_end_fun); 
  if (rc != SD_SUCCESS)
    {
    SXLOG_FLOW1 ("ERROR: parsing failed, return code: '%d'", rc);
    }

  chk_free (cfgData);
  return (rc);
  }
  
/************************************************************************/
/*			    sx_parseEx_mt				*/
/* obsolete function required to build AX-S4 ICCP w/Marben stack	*/
/************************************************************************/
ST_RET sx_parseEx_mt (ST_CHAR *fileName, ST_INT numItems,
                 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
{
SX_DEC_CTRL *sxDecCtrl;
ST_RET rc;
FILE *fp;

  SXLOG_FLOW1 ("Start Decoding XML file %s", fileName);
  if ((fp = fopen (fileName,"r"))==NULL)
    {
    SXLOG_FLOW1 ("XML File (%s) Open Error",fileName);
    return (SX_FILE_NOT_FOUND);
    }
  sxDecCtrl = (SX_DEC_CTRL *) calloc (1, sizeof (SX_DEC_CTRL));
            /* !we need to use here the system calloc */
  if (sxDecCtrl == NULL)
    {
    fclose (fp);  /* close file opened above */
    return (SD_FAILURE);
    }

  sx_push (sxDecCtrl, numItems, itemTbl, SD_FALSE);
  sxDecCtrl->fp = fp;
  sxDecCtrl->ignoreNS = sxIgnoreNS;
  sxDecCtrl->usr = usr;
  sxDecCtrl->u_sx_el_start = u_sx_el_start_fun;
  sxDecCtrl->u_sx_el_end   = u_sx_el_end_fun;
 
#if defined(USE_EXPAT)
  sx_rip_xml_file (sxDecCtrl);
#else
  sx_rip_xml (sxDecCtrl);
#endif

  if (sxDecCtrl->errCode == 0)
    {
    if (sxDecCtrl->xmlNestLevel != 0)
      {
      sxDecCtrl->errCode = SX_XML_MALFORMED;
      SXLOG_NERR0 ("Invalid XML nesting");
      }
    }

  rc = sxDecCtrl->errCode;

  free (sxDecCtrl);

  fclose (fp);
  return (rc);
}

/************************************************************************/
/*				sx_parse_mt				*/
/* Note: when making changes/correction to this function revise also	*/
/*       other sx_parse functions.					*/
/* Decode the XML located in memory (thread safe version).		*/
/************************************************************************/
ST_RET sx_parse_mt (ST_LONG lMsgLen, ST_CHAR *xml, ST_INT numItems, 
		 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
{
SX_DEC_CTRL *sxDecCtrl;
ST_RET rc;

  SXLOG_FLOW0 ("Start Decoding XML");
  SXLOG_FLOWH ((ST_INT) lMsgLen, xml);

  sxDecCtrl = (SX_DEC_CTRL *) calloc (1, sizeof (SX_DEC_CTRL));
			/* !we need to use here the system calloc */
  if (sxDecCtrl == NULL)
    return (SD_FAILURE);

  sx_push (sxDecCtrl, numItems, itemTbl, SD_FALSE);
  sxDecCtrl->xmlStart = xml;
  sxDecCtrl->xmlLen = lMsgLen;
  sxDecCtrl->ignoreNS = sxIgnoreNS;
  sxDecCtrl->usr = usr;
  sxDecCtrl->u_sx_el_start = u_sx_el_start_fun;
  sxDecCtrl->u_sx_el_end   = u_sx_el_end_fun;
 
#if defined(USE_EXPAT)
  sx_rip_xml_mem (sxDecCtrl);
#else
  sx_rip_xml (sxDecCtrl);
#endif

  if (sxDecCtrl->xmlNestLevel != 0 && sxDecCtrl->errCode == SD_SUCCESS)
    {
    sxDecCtrl->errCode = SX_XML_MALFORMED;
    SXLOG_NERR0 ("Invalid XML nesting");
    }

  rc = sxDecCtrl->errCode;

  free (sxDecCtrl);

  return (rc);
}

/************************************************************************/
/*			sxStartElement 					*/
/************************************************************************/

ST_VOID sxStartElement (SX_DEC_CTRL *sxDecCtrl) 
  {
SX_ELEMENT *item;
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *tag;
ST_RET rc = SD_FAILURE;
ST_INT stackLevelSave;
ST_INT *numOccPtr;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  tag = sxDecElInfo->tag;
  SXLOG_DEC1 ("Start element '%s'", tag);
  /* CRITICAL: This function usually increments "xmlNestLevel" so now	*/
  /*           it must be less than the maximum allowed.		*/
  if (sxDecCtrl->xmlNestLevel >= SX_MAX_XML_NEST-1)
    {
    sxDecCtrl->errCode = SX_XML_NEST_TOO_DEEP;
    SXLOG_ERR1 ("Start tag '%s' exceeds max nesting level", tag);
    return;
    }
  if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
    {
    return;
    }

  item = _uibed_find_element (sxDecCtrl, tag, &numOccPtr);
  
  stackLevelSave = sxDecCtrl->itemStackLevel;
  while (item == NULL && sxDecCtrl->itemStackLevel > 0)
    {
    if (sxDecCtrl->auto_pop[sxDecCtrl->itemStackLevel-1] == SD_TRUE)
      {
      _sx_pop (sxDecCtrl, SD_TRUE);
      item = _uibed_find_element (sxDecCtrl, tag, &numOccPtr);
      }
    else
      break;
    }

  if (item != NULL)
    {
    if (*numOccPtr != 0 && ((item->elementFlags & SX_ELF_RPT) == 0))
      {
      sxDecCtrl->errCode = SX_DUPLICATE_NOT_ALLOWED;
      SXLOG_NERR1 ("Duplicate of element '%s' not allowed",  tag);
      return;       
      }
    ++(*numOccPtr);
    if (*numOccPtr > 1)
      {
      SXLOG_CDEC1 ("Number occurences: %d", *numOccPtr);
      }

  /* Save the item for later */
    ++sxDecCtrl->xmlNestLevel;
    sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel] = item;

  /* Call the user function, if there is one ... */
    sxDecCtrl->item = item;
    sxDecCtrl->reason = SX_ELEMENT_START;
    if ((item->elementFlags & SX_ELF_CSTART) != 0)
      {
      if (item->funcPtr != NULL)
        {
	sxDecCtrl->elUser = item->user;
        (item->funcPtr)(sxDecCtrl);
	}
      else
        {
        SXLOG_CDEC0 ("No state function for this element");
        }
      }
    }
  else if (sxDecCtrl->u_sx_el_start != NULL)
    {
    sxDecCtrl->itemStackLevel = stackLevelSave;

    if (sxDecCtrl->u_sx_el_start != NULL)
      rc = (*(sxDecCtrl->u_sx_el_start)) (sxDecCtrl, tag);
    if (rc == SD_SUCCESS)
      {
      ++sxDecCtrl->xmlNestLevel;
      sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel] = NULL;
      }
    else
      {
      sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
      SXLOG_CDEC1 ("u_sx_el_start failed for element '%s'", tag);
      }
    }
  else
    {
    sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
    SXLOG_NERR1 ("Could not find element '%s' in element table", tag);
    }
  }

/************************************************************************/
/*			sxEndElement 					*/
/************************************************************************/

ST_VOID sxEndElement (SX_DEC_CTRL *sxDecCtrl)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
SX_ELEMENT *item;
ST_CHAR *tag;
ST_RET rc = SD_FAILURE;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  tag = sxDecElInfo->tag;
  SXLOG_DEC1 ("End element   '%s'", tag);
  /* CRITICAL: This function usually decrements "xmlNestLevel" so now	*/
  /*           it must be > 0.						*/
  if (sxDecCtrl->xmlNestLevel <= 0)
    {
    sxDecCtrl->errCode = SX_XML_MALFORMED;
    SXLOG_ERR1 ("Unexpected End tag '%s'. Invalid nesting.", tag);
    return;
    }
  if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
    {
    return;
    }

  item = sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel];
  --sxDecCtrl->xmlNestLevel;

  if (item != NULL)
    {
    if (strcmp (tag, item->tag) != 0) /* verify end tag */
      {
      sxDecCtrl->errCode = SX_XML_MALFORMED;
      SXLOG_NERR2 ("XML malformed: found </%s>, expected </%s>", tag, item->tag);
      }
    else
      {
      if ((item->elementFlags & SX_ELF_CEND) != 0)
        {
        sxDecCtrl->item = item;
        sxDecCtrl->reason = SX_ELEMENT_END;
        if (item->funcPtr != NULL)
	  {
  	  sxDecCtrl->elUser = item->user;
          (item->funcPtr)(sxDecCtrl);
	  }
        else
          {
          SXLOG_CDEC0 ("No state function for this element");
          }
        }
      }
    }
  else
    {
    if (sxDecCtrl->u_sx_el_end != NULL)
      {
      rc = (*(sxDecCtrl->u_sx_el_end)) (sxDecCtrl, sxDecCtrl->sxDecElInfo.tag);
      if (rc != SD_SUCCESS)
        {
        sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
        SXLOG_NERR1 ("u_sx_el_end failed for element '%s'", tag);
        }
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*				sx_push 				*/
/************************************************************************/

ST_VOID sx_push (SX_DEC_CTRL *sxDecCtrl, ST_INT numItems, SX_ELEMENT *itemTbl,
		ST_BOOLEAN auto_pop)
  {
ST_INT i;
SX_ELEMENT_TBL_CTRL *itemTblCtrl;
ST_INT *numOccTbl;

/* Do some sanity checks first */
  if (sxDecCtrl->itemStackLevel >= SX_MAX_STACK_LEVEL)
    {
    sxDecCtrl->errCode = SX_XML_NEST_TOO_DEEP;
    return;
    }
  if (numItems > SX_MAX_ITEMS_PER_TABLE)
    {
    sxDecCtrl->errCode = SX_ELEMENT_TBL_TOO_BIG;
    return;
    }

  itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel];
  numOccTbl = itemTblCtrl->numOccTbl;
  
  itemTblCtrl->itemTbl= itemTbl;
  itemTblCtrl->numItems= numItems;
  sxDecCtrl->auto_pop[sxDecCtrl->itemStackLevel] = auto_pop;
  ++sxDecCtrl->itemStackLevel;

/* reset the numOCc elements */
  for (i = 0; i < numItems; ++i)
    numOccTbl[i] = 0;
  }

/************************************************************************/
/*				sx_pop 					*/
/************************************************************************/

ST_VOID sx_pop (SX_DEC_CTRL *sxDecCtrl)
  {
  _sx_pop (sxDecCtrl, SD_FALSE);
  }


/************************************************************************/
/*			_sx_pop						*/
/************************************************************************/

static ST_VOID _sx_pop (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN auto_pop)
  {
SX_ELEMENT_TBL_CTRL *itemTblCtrl;
SX_ELEMENT *item;
ST_INT i;
ST_INT *numOccTbl;

/* Check for mandatory elements */
  if (sxDecCtrl->itemStackLevel > 0)
    {
    --sxDecCtrl->itemStackLevel;
  /* If auto-popping AND have a unknown element handler, don't check mandatory */
    if (auto_pop && sxDecCtrl->u_sx_el_start != NULL)
      return;

    itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel];
    numOccTbl = itemTblCtrl->numOccTbl;
    for (i = 0; i < itemTblCtrl->numItems; ++i)
      {
      item = &itemTblCtrl->itemTbl[i];
      if (numOccTbl[i] == 0 && ((item->elementFlags & SX_ELF_OPT) == 0))
        {
        sxDecCtrl->errCode = SX_REQUIRED_TAG_NOT_FOUND;
        SXLOG_NERR1 ("Mandatory element '%s' not found",  item->tag);
        break;       
        }
      }
    }
  }


/************************************************************************/
/************************************************************************/

SX_ELEMENT *_uibed_find_element (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag, ST_INT **numOccPtrOut)
  {
SX_ELEMENT_TBL_CTRL *itemTblCtrl;
ST_INT numItems;
SX_ELEMENT *item;
ST_INT i;

  itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel-1];
  item = itemTblCtrl->itemTbl;
  numItems = itemTblCtrl->numItems;

/* See if this element is in our table */
  for (i = 0; i < numItems; ++i, ++item)
    {
    if (strcmp (tag, item->tag) == 0)
      {
      *numOccPtrOut = &itemTblCtrl->numOccTbl[i];
      return (item);
      }
    }
  return (NULL);
  }



/************************************************************************/
/************************************************************************/
/************************************************************************/

ST_RET sx_get_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut)
  {
ST_CHAR *src;
ST_INT len;
ST_INT rc;

  rc = sx_find_element_contents (sxDecCtrl, &src, &len);
  if (rc != SD_SUCCESS)
    return (rc);

  if (destLen < len)
    {
    SXLOG_NERR0("sx_get_element_contents: dest too small");
    SXLOG_NERR1 ("Error: sx_get_element_contents: dest too small for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }

  memcpy (destBuf, src, len);
  *lenOut = len;
  return (SD_SUCCESS);
  }

/************************************************************************/

ST_RET sx_find_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR **elStartOut, ST_INT *lenOut)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *start;
ST_CHAR *end;
ST_CHAR endTagBuf[SX_MAX_TAG_LEN+3];	/* tag size is SX_MAX_TAG_LEN. Add 3 for </>	*/
ST_INT len;
#if defined(USE_EXPAT)
int offset;
int size;
const char *ptr;

  /* Doesn't work if reading one buffer at a time from a file.	*/
  if (sxDecCtrl->fp)
    {
    SXLOG_ERR0 ("sx_find_element_contents only works if entire XML is in memory. Use sx_parseExx_mt or sx_parse_mt.");
    return (SD_FAILURE);
    }

  /* Set "start" ptr to point after end of this start tag.	*/
  ptr = XML_GetInputContext(sxDecCtrl->parser, &offset, &size);
  ptr += offset;	/* point to current position in XML buffer	*/
			/* should be beginning of start tag		*/
  start = strchr (ptr, '>') + 1;	/* point after end of start tag	*/

  /* save nest level to help find corresponding end tag	*/
  sxDecCtrl->skipNestLevel = sxDecCtrl->xmlNestLevel;
  /* Change handlers to skip to end of this element (look for end tag).	*/
  XML_SetCharacterDataHandler(sxDecCtrl->parser, NULL);	/* ignore element data	*/
  XML_SetElementHandler(sxDecCtrl->parser, expatHandlerStartSkip, expatHandlerEndSkip);
  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
#else
  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  start = sxDecCtrl->xmlPos;
#endif
/* Check to see if this is an empty element */
  if (*(start - 2) == '/')
    {
    *lenOut = 0;
    return (SD_SUCCESS);
    }
    
  *elStartOut = start;
  len = sxDecCtrl->xmlLen - (int) (start - sxDecCtrl->xmlStart);

/* We need to find the closing element for this start element */
  endTagBuf[0] = '<';
  endTagBuf[1] = '/';
  strcpy (&endTagBuf[2], sxDecElInfo->tag);
  strcat (endTagBuf, ">");

  end = strnstr (start, endTagBuf, len);
  if (end == NULL)
    {
    SXLOG_NERR1("Can't find end tag '%s'", sxDecElInfo->tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  *lenOut = (int) (end - start);

#if !defined(USE_EXPAT)		/* with Expat, can't set or use xmlPos	*/
  sxDecCtrl->xmlPos = end;
#endif

  SXLOG_CDEC0("sx_find_element_contents got data:");
  SXLOG_DECH (*lenOut, *elStartOut);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			sx_get_entity 					*/
/************************************************************************/

ST_RET sx_get_entity (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut)
  {
#if defined(USE_EXPAT)
  if (destLen < sxDecCtrl->entityLen)
    {
    SXLOG_NERR2 ("Error: sx_get_entity: dest too small (max %d bytes expected) for tag '%s'", destLen, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    *lenOut = 0;	/* return empty buf	*/
    return (SD_FAILURE);
    }
  else
    {
    memcpy (destBuf, sxDecCtrl->entityBuf, sxDecCtrl->entityLen);
    *lenOut = sxDecCtrl->entityLen;
    }
  if (*lenOut == 0)
    SXLOG_CDEC0("sx_get_entity (no data)");
  else
    {
    SXLOG_CDEC0("sx_get_entity got data:");
    SXLOG_DECH (*lenOut, destBuf);
    }
  return (SD_SUCCESS);
#else	/* !USE_EXPAT	*/
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_INT i;
ST_CHAR *dest;
ST_CHAR *src;
ST_CHAR *end;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  src = sxDecElInfo->entityStart;
  dest = destBuf;
  end = sxDecElInfo->entityEnd;
  for (i = 0; i < destLen && src < end; ++i, ++src, ++dest)
    *dest = *src;

  *lenOut = i;

  if (src < end)
    {
    SXLOG_NERR2 ("Error: sx_get_entity: dest too small (max %d bytes expected) for tag '%s'", destLen, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }

  SXLOG_CDEC0("sx_get_entity got data:");
  SXLOG_DECH (i, destBuf);
  return (SD_SUCCESS);
#endif	/* !USE_EXPAT	*/
  }


/************************************************************************/
/*                       sx_get_bool					*/
/************************************************************************/

ST_RET sx_get_bool (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *out_ptr)
  {
ST_INT d;
ST_RET rc;

  rc = sx_get_int (sxDecCtrl, &d);
  if (rc == SD_SUCCESS)
    {	/* convert int to boolean	*/
    if (d == 0)
      *out_ptr = SD_FALSE;
    else
      *out_ptr = SD_TRUE;
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_int					*/
/************************************************************************/

ST_RET sx_get_int (SX_DEC_CTRL *sxDecCtrl, ST_INT *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToSint (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to INT for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_float					*/
/************************************************************************/

ST_RET sx_get_float (SX_DEC_CTRL *sxDecCtrl, ST_FLOAT *out_ptr)
  {
ST_DOUBLE dbl;
ST_RET rc;

  rc = sx_get_double (sxDecCtrl, &dbl);
  if (rc == SD_SUCCESS)
    *out_ptr = (ST_FLOAT) dbl;	/* convert double to float	*/

  return (rc);
  }

/************************************************************************/
/*                       sx_get_double					*/
/************************************************************************/

ST_RET sx_get_double (SX_DEC_CTRL *sxDecCtrl, ST_DOUBLE *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToDouble (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to DOUBLE for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uchar					*/
/************************************************************************/

ST_RET sx_get_uchar (SX_DEC_CTRL *sxDecCtrl, ST_UCHAR *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%c", out_ptr));
  }

/************************************************************************/
/*                       sx_get_int16					*/
/************************************************************************/

ST_RET sx_get_int16 (SX_DEC_CTRL *sxDecCtrl, ST_INT16 *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToSint16 (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to INT16 for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uint16					*/
/************************************************************************/

ST_RET sx_get_uint16 (SX_DEC_CTRL *sxDecCtrl, ST_UINT16 *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToUint16 (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to UINT16 for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uint32					*/
/************************************************************************/

ST_RET sx_get_uint32 (SX_DEC_CTRL *sxDecCtrl, ST_UINT32 *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToUint32 (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to UINT32 for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uint32_hex				*/
/************************************************************************/

ST_RET sx_get_uint32_hex (SX_DEC_CTRL *sxDecCtrl, ST_UINT32 *out_ptr)
  {
ST_ULONG ul;
ST_RET   rc;

  if ((rc = sx_get_value (sxDecCtrl, "0x%lx", &ul)) == SD_SUCCESS)
    *out_ptr = (ST_UINT32) ul;
  return (rc);
  }
/************************************************************************/
/*                       sx_get_int32					*/
/************************************************************************/

ST_RET sx_get_int32 (SX_DEC_CTRL *sxDecCtrl, ST_INT32 *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToSint32 (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to INT32 for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_long					*/
/************************************************************************/

ST_RET sx_get_long (SX_DEC_CTRL *sxDecCtrl, ST_LONG *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToSlong (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to LONG for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_ulong					*/
/************************************************************************/

ST_RET sx_get_ulong (SX_DEC_CTRL *sxDecCtrl, ST_ULONG *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToUlong (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to ULONG for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uint					*/
/************************************************************************/

ST_RET sx_get_uint (SX_DEC_CTRL *sxDecCtrl, ST_UINT *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);	/* get string	*/
  if (rc == SD_SUCCESS)
    {
    rc = asciiToUint (str, out_ptr);			/* convert	*/
    if (rc)
      {
      SXLOG_ERR2 ("Can't convert string '%s' to UINT for element '%s'",
                  str, sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_value					*/
/************************************************************************/

ST_RET sx_get_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *format_string, ST_VOID *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

/* Convert to desired data format*/
/* Note: sscanf may return 'bad' value if the number in the str	*/
/*       exceeds the max value in format_string.		*/
  if (sscanf (str, format_string, out_ptr) != 1)
  /* we just want to get one value out of the string, any other ret is an error */
    {
    SXLOG_NERR1 ("Data Conversion Error for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       *sx_get_alloc_string				*/
/************************************************************************/

ST_RET sx_get_alloc_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  *strOut = M_STRDUP(NULL,str);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       *sx_get_string					*/
/************************************************************************/

ST_RET sx_get_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *dest, ST_INT *lenOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  if (*lenOut != 0)
    {
    if (strLen > *lenOut)
      {
      SXLOG_NERR1 ("Error: String too long for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }
    }

  sx_format_string_dec (dest, str);
  *lenOut = strLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_format_string_dec			        */
/************************************************************************/

ST_VOID sx_format_string_dec (ST_CHAR *dest, ST_CHAR *src)
  {
ST_CHAR *srcPtr;
ST_CHAR *srcPtrLast;
ST_CHAR *dstPtr;
ST_INT  diff;

  dstPtr = dest;
  srcPtrLast = src;
  srcPtr = strchr (src, '&');

  if (srcPtr == NULL)
    {
    strcpy (dest, src);
    return;
    }

  diff = (int) (srcPtr - srcPtrLast);
  strncpy (dstPtr, srcPtrLast, diff);
  dstPtr += diff;

  /* parse the source string and generate the dest string */
  while (srcPtr != NULL)
    {
    /* srcPtr points at a & */
    /* lets find out if the following characters are what we are looking for */
    if (strncmp (srcPtr, CODE_APOS, CODE_APOS_LEN) == 0)
      {
      *dstPtr++ = CHAR_APOS;
      srcPtr += CODE_APOS_LEN;
      }
    else if (strncmp (srcPtr, CODE_QUOT, CODE_QUOT_LEN) == 0)
      {
      *dstPtr++ = CHAR_QUOT;
      srcPtr += CODE_QUOT_LEN;
      }
    else if (strncmp (srcPtr, CODE_AMP, CODE_AMP_LEN) == 0)
      {
      *dstPtr++ = CHAR_AMP;
      srcPtr += CODE_AMP_LEN;
      }
    else if (strncmp (srcPtr, CODE_LT, CODE_LT_LEN) == 0)
      {
      *dstPtr++ = CHAR_LT;
      srcPtr += CODE_LT_LEN;
      }
    else if (strncmp (srcPtr, CODE_GT, CODE_GT_LEN) == 0)
      {
      *dstPtr++ = CHAR_GT;
      srcPtr += CODE_GT_LEN;
      }
    else
      {
      *dstPtr++ = '&';
      srcPtr++;
      }

    srcPtrLast = srcPtr;
    srcPtr = strchr (srcPtr, '&');

    if (srcPtr == NULL)
      {
      /* copy the remaining section of the string */
      strcpy (dstPtr, srcPtrLast);
      }
    else
      {
      diff = (int) (srcPtr - srcPtrLast);
      strncpy (dstPtr, srcPtrLast, diff);
      dstPtr += diff;
      }
    }
  }


/************************************************************************/
/************************************************************************/
/************************************************************************/
/*                       sx_get_bool_attr				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_bool_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_BOOLEAN *out_ptr)
  {
ST_INT d;
ST_RET rc;

  rc = sx_get_int_attr (sxDecCtrl, name, &d);
  if (rc == SD_SUCCESS)
    {	/* convert int to boolean	*/
    if (d == 0)
      *out_ptr = SD_FALSE;
    else
      *out_ptr = SD_TRUE;
    }

  return (rc);
  }

/************************************************************************/
/*                       sx_get_int_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_int_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT *out_ptr)
  {
ST_CHAR *str;
    
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToSint (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to INT for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_float_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_float_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_FLOAT *out_ptr)
  {
ST_DOUBLE dbl;
ST_RET rc;

  rc = sx_get_double_attr (sxDecCtrl, name, &dbl);
  if (rc == SD_SUCCESS)
    *out_ptr = (ST_FLOAT) dbl;	/* convert double to float	*/

  return (rc);
  }

/************************************************************************/
/*                       sx_get_double_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_double_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_DOUBLE *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToDouble (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to DOUBLE for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_uchar_attr 				*/
/************************************************************************/

ST_RET sx_get_uchar_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UCHAR *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%c", out_ptr));
  }

/************************************************************************/
/*                       sx_get_int16_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_int16_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT16 *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToSint16 (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to INT16 for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_uint16_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_uint16_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT16 *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToUint16 (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to UINT16 for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_uint32_attr				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_uint32_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT32 *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToUint32 (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to UINT32 for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_int32_attr				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_int32_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT32 *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToSint32 (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to INT32 for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_long_attr					*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_long_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_LONG *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToSlong (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to LONG for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_ulong_attr				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_ulong_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_ULONG *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToUlong (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to ULONG for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_uint_attr 				*/
/* RETURNS: SD_SUCCESS, SX_ERR_CONVERT, or SX_ERR_ATTR_NOT_FOUND.	*/
/************************************************************************/

ST_RET sx_get_uint_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT *out_ptr)
  {
ST_CHAR *str;
  if (sx_get_attr_ptr (sxDecCtrl, &str, name))		/* get string	*/
    return (SX_ERR_ATTR_NOT_FOUND);

  if (asciiToUint (str, out_ptr))			/* convert	*/
    {
    SXLOG_ERR2 ("Can't convert string '%s' to UINT for attr '%s'", str, name);
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    return (SX_ERR_CONVERT);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       XmlStringToDuration 				*/
/************************************************************************/

ST_RET XmlStringToDuration (ST_CHAR *src, SX_DURATION *out_ptr)
  {
ST_CHAR *p;
ST_CHAR temp[50]; /* arbitrary length */
ST_CHAR temp2[50]; /* arbitrary length */
ST_BOOLEAN foundP = SD_FALSE;
ST_BOOLEAN foundT = SD_FALSE;
ST_BOOLEAN foundDot = SD_FALSE;
ST_UINT i;
ST_BOOLEAN something = SD_FALSE;  /* Make sure we got SOMETHING out of */
				  /* this, any of the fields can be missing */

  out_ptr->inUse = SD_TRUE;
  p = src;
  memset (temp, 0, sizeof(temp));
  memset (temp2, 0, sizeof(temp2));
  for (i = 0; i < strlen(src); i++, p++)
    {
    /* check all error conditions I can think of */
    if ((*p == '-' && foundP) || 
        (isdigit(*p) && !foundP) ||
        (*p == 'Y' && !foundP) ||
        (*p == 'M' && !foundP) ||
        (*p == 'D' && !foundP) ||
        (!foundP && foundT) ||
        (*p == 'H' && !foundT) ||
        (*p == 'S' && !foundT) ||
	(*p != 'P' && *p != 'T' && isalpha(*p) && strlen(temp) == 0))
      return (SD_FAILURE);
      
    /* check for negative */
    else if (*p == '-')
      out_ptr->negative = SD_TRUE;
    else if (*p == '.')
      foundDot = SD_TRUE;
    /* We found a number */
    else if (isdigit(*p))
      {
      if (foundDot)
	strncat (temp2, p, 1);
      else
	strncat (temp, p, 1);
      }
    /* We found a letter */
    else if (isalpha(*p))
      {
      switch (*p)
	{
	case 'P':
	  foundP = SD_TRUE;
	  break;
	case 'T':
	  foundT = SD_TRUE;
	  break;
	case 'Y':
	  sscanf(temp, "%d", &out_ptr->years);
	  temp[0] = 0;
	  something = SD_TRUE;
	  break;
	case 'M':
	  if (foundT)
	    sscanf(temp, "%d", &out_ptr->minutes);
	  else
	    sscanf(temp, "%d", &out_ptr->months);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'D':
	  sscanf(temp, "%d", &out_ptr->days);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'H':
	  sscanf(temp, "%d", &out_ptr->hours);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'S':
	  sscanf(temp, "%d", &out_ptr->seconds);
	  temp[0] = 0;
	  sscanf(temp2, "%ld", &out_ptr->microseconds);
	  temp2[0] = 0;
  	  something = SD_TRUE;
	  break;
	default:
          return (SD_FAILURE);
	}
      }
    }
  if (something == SD_FALSE)
    return (SD_FAILURE);

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_duration_attr 				*/
/************************************************************************/

ST_RET sx_get_duration_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, SX_DURATION *out_ptr)
  {
ST_RET rc;
ST_CHAR *sxDuration;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &sxDuration, name);
  if (rc)
    return (rc);

  rc = XmlStringToDuration (sxDuration, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: String to Duration conversion for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
      
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_xtime_attr 				*/
/************************************************************************/

ST_RET sx_get_xtime_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, SX_DATE_TIME *out_ptr)
  {
ST_RET rc;
ST_CHAR *sxDateTime;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &sxDateTime, name);
  if (rc)
    return (rc);
    
  rc = XmlStringToUtcValue (sxDateTime, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: String to Value date/time conversion for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm_attr 				*/
/************************************************************************/

ST_RET sx_get_tm_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, struct tm *out_ptr)
  {
ST_RET rc;
ST_CHAR *theTime;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &theTime, name);
  if (rc)
    return (rc);
    
  rc = tstrStringToTm (theTime, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: String to Value (struct tm) date/time conversion for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			sx_get_attr_value 				*/
/************************************************************************/

ST_RET sx_get_attr_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, 
			  ST_CHAR *format_string, ST_VOID *out_ptr)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
    /* Convert to desired data format*/
      if (!sscanf (sxDecCtrl->sxDecElInfo.attr[i].value, 
			format_string, out_ptr))
        {
        SXLOG_NERR1 ("Error: Attribute Data Conversion Error for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       sx_get_attr					*/
/************************************************************************/

ST_RET sx_get_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *value, ST_CHAR *name)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
      strcpy (value, sxDecCtrl->sxDecElInfo.attr[i].value);
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       sx_get_attr_ptr				*/
/************************************************************************/

ST_RET sx_get_attr_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **ptrOut, ST_CHAR *name)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
      *ptrOut = sxDecCtrl->sxDecElInfo.attr[i].value;
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       *sx_get_bitstring				*/
/************************************************************************/

ST_RET sx_get_bitstring (SX_DEC_CTRL *sxDecCtrl, ST_INT *dest, ST_INT *lenOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_INT *bits;
ST_CHAR *pStr;
ST_RET rc;
ST_INT i;

  strLen = 0;
  bits = dest; 
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  if (*lenOut != 0)
    {
    if (strLen > *lenOut)
      {
      SXLOG_NERR1 ("Error: String too long for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }
    }

  pStr = str;
  pStr += strlen(str) - 1;
  for (i=0;i<(ST_INT)strlen(str);++i,--pStr)
    {
    if (strncmp(pStr, "0", 1) != 0 &&
        strncmp(pStr, "1", 1) != 0)
      {
      SXLOG_NERR1 ("Error: Not a bitstring for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }

    if (strncmp(pStr, "1", 1) == 0)
      {
      *bits |= (1 << i);
      }
    }

  *lenOut = strLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_string_ptr   				*/
/************************************************************************/

ST_RET sx_get_string_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut, ST_INT *lenOut)
  {
ST_INT vLen;
ST_RET rc;

  rc = sx_get_entity (sxDecCtrl, sxDecCtrl->elemBuf, sizeof(sxDecCtrl->elemBuf), &vLen);
  if (rc != SD_SUCCESS)
    return (SD_FAILURE);

/* Convert to desired data format*/
  sxDecCtrl->elemBuf[vLen] = 0;
  *strOut = sxDecCtrl->elemBuf;
  *lenOut = vLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_string_YesNo   				*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "Yes"/"No" and sets the dest to SD_TRUE/SD_FALSE respectively.	*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_BOOLEAN  *dest		pointer to dest to SD_TRUE/SD_FALSE	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode.				*/
/************************************************************************/
ST_RET sx_get_string_YesNo (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest,
                            ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "Yes") == 0)
      *dest = SD_TRUE;
    else if (stricmp (str, "No") == 0)
      *dest = SD_FALSE;
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (Yes/No expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }

  return (rc);
  }


/************************************************************************/
/*			sx_get_string_OnOff_mask			*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "On"/"Off" and sets/resets a maskBit in the mask.			*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_UINT     *mask		pointer to mask to set/reset a bit	*/
/*   ST_UINT      maskBit       mask bit to set/reset if ON/OFF	found	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode				*/
/************************************************************************/

ST_RET sx_get_string_OnOff_mask (SX_DEC_CTRL *sxDecCtrl, ST_UINT *mask,
                                ST_UINT maskBit, ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "On") == 0)
      {
      *mask |= maskBit;		/* turn on this bit in the mask		*/
      }						       
    else if (stricmp (str, "Off") == 0)
      {						       
      *mask &= ~maskBit;	/* turn off this bit in the mask	*/
      }
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (On/Off expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }
  return (rc);
  }

/************************************************************************/
/*			sx_get_string_OnOff_bool			*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "On"/"Off" and sets the dest to SD_TRUE/SD_FALSE respectively.	*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_BOOLEAN  *dest		pointer to dest to SD_TRUE/SD_FALSE	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode				*/
/************************************************************************/

ST_RET sx_get_string_OnOff_bool (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest,
                                ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "On") == 0)
      *dest = SD_TRUE;
    else if (stricmp (str, "Off") == 0)
      *dest = SD_FALSE;
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (On/Off expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_time					*/
/************************************************************************/
ST_RET sx_get_time (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr)
  {
ST_CHAR *str;
ST_RET rc;
time_t t;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);


  rc = tstrStringToTime (str, &t);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR2 ("Error: Time conversion (0x%04x) for tag '%s'", rc, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  *out_ptr = t;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm					*/
/************************************************************************/
ST_RET sx_get_tm (SX_DEC_CTRL *sxDecCtrl, struct tm *out_ptr)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  rc = tstrStringToTm (str, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR2 ("Error: Time conversion (0x%04x) for tag '%s'", rc, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm_ex					*/
/************************************************************************/
ST_RET sx_get_tm_ex (SX_DEC_CTRL *sxDecCtrl, struct tm *out_ptr, ST_LONG *microseconds)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;
char *periodLoc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  periodLoc = strchr (str, '.');
  /* Must convert fraction to microseconds BEFORE overwriting '.'	*/
  str_to_microsec (periodLoc, microseconds);	/* periodLoc may be NULL*/
  if (periodLoc != NULL)
    {
    *periodLoc = '\0';
    }

  rc = tstrStringToTm (str, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR2 ("Error: Time conversion (0x%04x) for tag '%s'", rc, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_duration				*/
/************************************************************************/
/* Store duration string found in an xml file into the structure	*/
/* "SX_DURATION".                               			*/
/************************************************************************/

ST_RET sx_get_duration (SX_DEC_CTRL *sxDecCtrl, SX_DURATION *sxDuration)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  memset (sxDuration, 0, sizeof (*sxDuration));
  		
  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  rc = XmlStringToDuration (str, sxDuration);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: String to Duration for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_xtime					*/
/************************************************************************/
/* Store date and time string found in an xml file into the structure   */
/* "SX_DATE_TIME" specifying the number of seconds from                 */
/* 1/1/1970 (UTC time), the number of microseconds in a decimal         */
/* fraction if it is specified and the number of minutes in the time    */
/* zone offset if it is specified.                                      */
/************************************************************************/

ST_RET sx_get_xtime (SX_DEC_CTRL *sxDecCtrl, SX_DATE_TIME *sxDateTime)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  memset (sxDateTime, 0, sizeof (*sxDateTime));
  		
  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  /* convert input xml string to date and time */
  /* store data in "SX_DATE_TIME" structure    */
  rc = XmlStringToUtcValue (str, sxDateTime);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: String to Value date/time conversion for tag '%s'", sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_time_ex					*/
/************************************************************************/
ST_RET sx_get_time_ex (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr, ST_LONG *microseconds)
  {
ST_CHAR *str;
char *periodLoc;
ST_RET rc;
time_t t;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  periodLoc = strchr (str, '.');
  /* Must convert fraction to microseconds BEFORE overwriting '.'	*/
  str_to_microsec (periodLoc, microseconds);	/* periodLoc may be NULL*/
  if (periodLoc != NULL)
    {
    *periodLoc = '\0';
    }

  rc = tstrStringToTime (str, &t);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR2 ("Error: Time conversion (0x%04x) for tag '%s'", rc, sxDecCtrl->sxDecElInfo.tag);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  *out_ptr = t;

  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* DEBUG: could easily change code to use only one of these 2 macros.	*/
#define SX_RIP_NOT_DONE	(!sxDecCtrl->termFlag && xml < xmlEnd)

#define SX_RIP_DONE	(sxDecCtrl->termFlag || xml >= xmlEnd)


#if !defined(USE_EXPAT)
/************************************************************************/
/*			sx_rip_xml 					*/
/************************************************************************/

ST_RET sx_rip_xml (SX_DEC_CTRL *sxDecCtrl)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *xml;
ST_INT  nestLevel;
ST_CHAR *tagDest;
ST_CHAR *attribNameDest;
ST_CHAR *attribValDest;
ST_CHAR attribValCopy[SX_MAX_ATTR_VALUE];
ST_INT len;
ST_LONG xmlLen;
ST_CHAR *xmlEnd = NULL;		/* init to avoid compiler warning	*/
ST_CHAR c,quotes_ch;
ST_BOOLEAN bEmptyTag;
ST_BOOLEAN sawStartTag = SD_FALSE;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  
  bEmptyTag = SD_FALSE;
  
  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  xml = sxDecCtrl->xmlStart;
  xmlLen = sxDecCtrl->xmlLen;
  xmlEnd = xml + xmlLen;

  while (SX_RIP_NOT_DONE)
    {
/* Find a begin or end tag */
    while (*xml != '<' && SX_RIP_NOT_DONE)
      xml++;

    if (SX_RIP_DONE)
      break;
   
   sawStartTag = SD_TRUE;

   sxDecElInfo->tagStart = xml;
   
/* OK, this should be the start of a start tag, an end tag, or a comment */
/* or block of binary CDATA						 */
    xml++;
    if (strncmp (xml, "!--", 3) == 0 || *xml == '?') /* Comment */
      {
      while (strncmp (xml, "-->", 3) != 0 && 
           strncmp (xml, "?>", 2)  != 0 &&
           SX_RIP_NOT_DONE)
        {
        xml++;
        }
      }
    else if (strncmp (xml, "![CDATA[", 8) == 0) /* CDATA */
      {
      while (strncmp (xml, "]]>", 3) != 0 &&
	     SX_RIP_NOT_DONE)
	{
	xml++;
	}
      }
    else if (strncmp (xml, "!DOCTYPE", 8) == 0) /* DocType */
      {
      nestLevel = 0;
      while (SD_TRUE)
        {
        if (*xml == '>')
          {
          if (nestLevel == 0)
            break;
          else
            --nestLevel;
          }
        if (*xml == '<')
          ++nestLevel;

        xml++;
        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find DOCTYPE end");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
	  }
	}
      xml++;
      }
    else if (*xml != '/')	/* Begin tag */
      {
    /* We have a element tag start, get the tag  first  */
      tagDest = sxDecElInfo->tag;
      len = 0;
      while (SX_RIP_NOT_DONE)
        {
        c = *xml;
        if (c == '>' || c == '/' || isspace ((int)(unsigned char)c)) 	/* Found the end of the tag *///MEMO: isspace����ת�����������ʹ��� [2016-8-18 18:20 �ۿ���]
          break;

        *(tagDest++) = c;
        ++len;
        if (len >= SX_MAX_TAG_LEN)
          {
          SXLOG_NERR0 ("SX decode error: tag too long");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        xml++;
	if (sxDecCtrl->ignoreNS && c == ':')
	  {
          tagDest = sxDecElInfo->tag;
      	  len = 0;
	  }
        }
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      *tagDest = 0;		/* terminate the tag */

    /* Now look for attributes */         
      sxDecElInfo->attrCount = 0;
      while (*xml != '>' && *xml != '/') /* we could have attributes! */
        {
      /* skip any whitespace before the start of the attribute name */
	while (isspace ((int)(unsigned char)*xml) && SX_RIP_NOT_DONE)//MEMO: isspace����ת�����������ʹ��� [2016-8-18 18:20 �ۿ���]
          xml++;

        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find attribute name");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }

        if (*xml != '>' && *xml != '/')
          {
          if (sxDecElInfo->attrCount >= SX_MAX_ATTRIB)
            {
            SXLOG_NERR0 ("SX decode error: too many attributes. Look at SX_MAX_ATTRIB define");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

        /* This should be the start of an attribute name */ 
          attribNameDest = sxDecElInfo->attr[sxDecElInfo->attrCount].name;
          len = 0;
          while (SX_RIP_NOT_DONE)
            {
            c = *xml;
            if (c == '=' || isspace ((int)(unsigned char)c))	/* Found the end of the attribute name *///MEMO: isspace����ת�����������ʹ��� [2016-8-18 18:20 �ۿ���]
              break;

            if (c == '>') /* this isn't really an attribute like we first thought */
              break;
       
	    /* copy this char. isspace check above stops on first white space*/
            *(attribNameDest++) = c;
            ++len;

            if (len >= SX_MAX_ATTR_NAME)
              {
              SXLOG_NERR0 ("SX decode error: attribute name too long");
              sxDecCtrl->errCode = SD_FAILURE;
              return (SD_FAILURE);
              }
            xml++;
	    if (sxDecCtrl->ignoreNS && c == ':')
	      {
              attribNameDest = sxDecElInfo->attr[sxDecElInfo->attrCount].name;
              len = 0;
	      }
            }

          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find attribute name end");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

          if (*xml == '>') /* this isn't really an attribute like we first thought */
            break;

          *attribNameDest = 0;		/* terminate the attrib name */

        /* skip to the attribute '=' */
          while (*xml != '=' && SX_RIP_NOT_DONE)
            xml++;
          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find attribute '='");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }
          xml++;
	  
          /* skip white space after '=' */
          while (isspace ((int)(unsigned char)*xml) && SX_RIP_NOT_DONE)//MEMO: isspace����ת�����������ʹ��� [2016-8-18 18:20 �ۿ���]
            xml++;
	    
          /* OK, get the attrib value */
          if (*(xml) != '"' && *(xml) != '\'')		/* skip the opening " */
            {
            SXLOG_NERR0 ("SX decode error: could not find leading attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }
		  quotes_ch = *(xml);//MEMO: ��¼attr����ֵ���������ֹ˫�����뵥���Ż���ʱ���� [2016-8-18 18:36 �ۿ���]
          xml++;
        
           /* At the start of the attribute value */ 
          attribValDest = sxDecElInfo->attr[sxDecElInfo->attrCount].value;
          len = 0;
          while (SX_RIP_NOT_DONE)
            {
            c = *xml;
            //MEMO:  [2016-8-18 18:37 �ۿ���]if (c == '"' || c == '\'')	/* Found the end of the attrib */
			if (c == quotes_ch)//MEMO: ��ֹ˫�����뵥���Ż���ʱ���� [2016-8-18 18:38 �ۿ���]
              break;
          
            if (c != 9 && c != 10 && c != 13) /* dont include these characters */
              {
              *(attribValDest++) = c;
              ++len;
              }

            if (len >= SX_MAX_ATTR_VALUE)
              {
              SXLOG_NERR0 ("SX decode error: attribute value too long");
              sxDecCtrl->errCode = SD_FAILURE;
              return (SD_FAILURE);
              }
            xml++;
            }
          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find closing attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

          //MEMO:  [2016-8-18 18:37 �ۿ���]if (*(xml) != '"' && *(xml) != '\'')		/* skip the closing " */
		  if(*(xml) != quotes_ch)//MEMO: ��ֹ˫�����뵥���Ż���ʱ���� [2016-8-18 18:38 �ۿ���]
            {
            SXLOG_NERR0 ("SX decode error: could not find closing attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }
          xml++;

          *attribValDest = 0;	/* terminate the attrib value */
          strcpy (attribValCopy, sxDecElInfo->attr[sxDecElInfo->attrCount].value);
          sx_format_string_dec (sxDecElInfo->attr[sxDecElInfo->attrCount].value, attribValCopy);
          ++sxDecElInfo->attrCount;
          }

        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find tag end");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        }

      /* Could be empty tag */
      if (*xml == '/')
	{
	bEmptyTag = SD_TRUE;
	while (*xml != '>')
          xml++;
	}
      xml++; /* skip the '>' */

    /* OK, now call the element start function */
      sxDecElInfo->entityStart = xml;
      sxDecElInfo->entityEnd = xml;
      sxDecCtrl->xmlPos  = xml;	/* Save current dec position 	*/

      sxStartElement (sxDecCtrl); 
      /* Fail on any error except convert error */
      if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
	{
	return (SD_FAILURE);
	}

      if (bEmptyTag)
	{
        /* OK, now call the element end function */
	sxEndElement (sxDecCtrl);
	bEmptyTag = SD_FALSE;
	}

      /* CRITICAL: sxStartElement may call sx_find_element_contents which*/
      /*           changes "sxDecCtrl->xmlPos". Change "xml" to match.	*/
      xml = sxDecCtrl->xmlPos;
      }
    else		/* End tag */
      {
      sxDecElInfo->entityEnd = xml - 1;
      xml++;
      tagDest = sxDecElInfo->tag;
      len = 0;
      while (SX_RIP_NOT_DONE)
        {
        c = *xml;
        if (c == '>' || isspace ((int)(unsigned char)c))	/* Found the end of the tag *///MEMO: isspace����ת�����������ʹ��� [2016-8-18 18:20 �ۿ���]
          break;

        *(tagDest++) = c;
        ++len;
        if (len >= SX_MAX_TAG_LEN)
          {
          SXLOG_NERR0 ("SX decode error: tag too long");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        xml++;
	if (sxDecCtrl->ignoreNS && c == ':')
	  {
          tagDest = sxDecElInfo->tag;
          len = 0;
	  }
        }
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      *tagDest = 0;		/* terminate the tag */


      /* any white space up to the end of the tag name */
      while (*xml != '>' && SX_RIP_NOT_DONE)
        xml++;
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      xml++;

      sxDecCtrl->xmlPos  = xml;	/* Save current dec position 	*/
      
    /* OK, now call the element end function */
      sxEndElement (sxDecCtrl);
      /* Fail on any error except convert error */
      if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
	{
	return (SD_FAILURE);
	}
      }
    }
    
  if (!sawStartTag)
    {
    SXLOG_ERR0 ("SX decode error: could not find start tag");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  else
    return (SD_SUCCESS);
  }

#else	/* USE_EXPAT	*/


/************************************************************************/
/*			expatHandlerStart				*/
/* Normal 'start tag' handler.						*/
/* Log and set "errCode" if any string is too long to be stored.	*/
/************************************************************************/
static void XMLCALL expatHandlerStart(void *userData, const char *el, const char **attr)
  {
  SX_DEC_CTRL *sxDecCtrl = (SX_DEC_CTRL *) userData;
  int i;
  const ST_CHAR *ptr;

  if (!sxDecCtrl->termFlag)
    {
    /* Copy tag to sxDecCtrl	*/
    /* If ignoreNS flag set, ignore namespace prefix on tag.	*/
    if (sxDecCtrl->ignoreNS)
      {
      /* If ':' in the string, copy only text after ':'	*/
      if ((ptr = strrchr (el, ':')) != NULL)
        ptr += 1;	/* point after ':'	*/
      else
        ptr = el;
      }
    else
      ptr = el;
  
    if (strlen (ptr) < SX_MAX_TAG_LEN)
      strcpy (sxDecCtrl->sxDecElInfo.tag, ptr);
    else
      {
      SXLOG_ERR1 ("start tag '%s' too long. Can't be stored.", ptr);
      sxDecCtrl->errCode = SD_FAILURE;
      }
  
    /* Copy attributes to sxDecCtrl	*/
    for (i = 0; attr[i]; i += 2)
      {
      if (strlen (attr[i]) < SX_MAX_ATTR_NAME)
        strcpy (sxDecCtrl->sxDecElInfo.attr[i/2].name, attr[i]);
      else
        {
        SXLOG_ERR1 ("attr name '%s' too long. Can't be stored.", ptr);
        sxDecCtrl->errCode = SD_FAILURE;
        }
  
      if (strlen (attr[i+1]) < SX_MAX_ATTR_VALUE)
        strcpy (sxDecCtrl->sxDecElInfo.attr[i/2].value, attr[i+1]);
      else
        {
        SXLOG_ERR1 ("attr value '%s' too long. Can't be stored.", ptr);
        sxDecCtrl->errCode = SD_FAILURE;
        }
      }
    sxDecCtrl->sxDecElInfo.attrCount = i/2;
    sxDecCtrl->entityLen = 0;	/* reset entityLen	*/
    sxStartElement (sxDecCtrl);
    /* Stop parsing if sxStartElement changed "errCode" to bad value.*/
    /* This should cause the line number to be logged.		*/
    if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
      XML_StopParser(sxDecCtrl->parser, XML_FALSE);	/* NOT resumable*/
    }	/* !termFlag	*/
  }

/************************************************************************/
/*			expatHandlerEnd					*/
/* Normal 'end tag' handler.						*/
/************************************************************************/
static void XMLCALL expatHandlerEnd(void *userData, const char *el)
  {
  SX_DEC_CTRL *sxDecCtrl = (SX_DEC_CTRL *) userData;

  if (!sxDecCtrl->termFlag)
    {
    strcpy (sxDecCtrl->sxDecElInfo.tag, el);
    sxEndElement (sxDecCtrl);
    sxDecCtrl->entityLen = 0;	/* reset entityLen	*/
    /* Stop parsing if sxEndElement changed "errCode" to bad value.*/
    /* This should cause the line number to be logged.		*/
    if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
      XML_StopParser(sxDecCtrl->parser, XML_FALSE);	/* NOT resumable*/
    }	/* !termFlag	*/
  }

/************************************************************************/
/*			expatHandlerData				*/
/* Normal data handler.							*/
/* Log and set "errCode" if data is too long to be stored.		*/
/************************************************************************/
static void XMLCALL expatHandlerData(void *userData,
                                    const XML_Char *s,
                                    int len)
  {
  SX_DEC_CTRL *sxDecCtrl = (SX_DEC_CTRL *) userData;
  ST_INT sizeNeeded;	/* buffer size needed to store this data	*/

  if (!sxDecCtrl->termFlag)
    {
    sizeNeeded = len + sxDecCtrl->entityLen;

    /* Save data to access with "sx_get_entity" later.	*/
    if (sizeNeeded > sxDecCtrl->entityBufSize)
      {
      /* Reallocate buffer twice as big as currently needed.	*/
      sxDecCtrl->entityBufSize = sizeNeeded * 2;
      SXLOG_FLOW1 ("entity buffer too small. Reallocating entity buffer size = %d", sxDecCtrl->entityBufSize);
      sxDecCtrl->entityBuf = chk_realloc (sxDecCtrl->entityBuf, sxDecCtrl->entityBufSize);
      }

    memcpy (&(sxDecCtrl->entityBuf [sxDecCtrl->entityLen]), s, len);
    sxDecCtrl->entityLen += len;
    }	/* !termFlag	*/
  }

/************************************************************************/
/*			expatHandlerStartSkip				*/
/* The 'start tag' handler when tags are being skipped.			*/
/************************************************************************/
static void XMLCALL expatHandlerStartSkip(void *userData, const char *el, const char **attr)
  {
  SX_DEC_CTRL *sxDecCtrl = (SX_DEC_CTRL *) userData;

  if (!sxDecCtrl->termFlag)
    {
    /* Not calling normal start funct, so must increment nest level here*/
    sxDecCtrl->xmlNestLevel++;
    }	/* !termFlag	*/
  }

/************************************************************************/
/*			expatHandlerEndSkip				*/
/* The 'end tag' handler when tags are being skipped.			*/
/************************************************************************/
static void XMLCALL expatHandlerEndSkip(void *userData, const char *el)
  {
  SX_DEC_CTRL *sxDecCtrl = (SX_DEC_CTRL *) userData;

  if (!sxDecCtrl->termFlag)
    {
    if (sxDecCtrl->xmlNestLevel == sxDecCtrl->skipNestLevel)
      {
      SXLOG_DEC1 ("End element   '%s' (contents skipped)", el);
      /* Go back to normal handlers.	*/
      XML_SetCharacterDataHandler(sxDecCtrl->parser, expatHandlerData);
      XML_SetElementHandler(sxDecCtrl->parser, expatHandlerStart, expatHandlerEnd);
      }

    /* Not calling normal end funct, so must decrement nest level here*/
    sxDecCtrl->xmlNestLevel--;
    }	/* !termFlag	*/
  }
/************************************************************************/
/*			setup_expat					*/
/* NOTE: If "sxDecCtrl->termFlag" is set by user (i.e. error found),	*/
/*       let parse complete, but ignore data (see "expatHandler*").	*/
/************************************************************************/
static XML_Parser setup_expat(SX_DEC_CTRL *sxDecCtrl)
  {
  XML_Parser parser = XML_ParserCreate(NULL);
  if (! parser)
    {
    SXLOG_ERR0 ("Couldn't allocate memory for XML parser");
    return (parser);
    }
  XML_SetUserData (parser, sxDecCtrl);	/* passes sxDecCtrl to handlers	*/
  XML_SetCharacterDataHandler(parser, expatHandlerData);
  XML_SetElementHandler(parser, expatHandlerStart, expatHandlerEnd);
  sxDecCtrl->parser = parser;	/* CRITICAL: save parser to use in callbacks*/

  return (parser);
  }
/************************************************************************/
/*			sx_rip_xml_file					*/
/* Parse XML from a file. The caller must open a file			*/
/* and store the file ptr in "sxDecCtrl->fp". It reads one segment	*/
/* at a time from the file and passes it to XML_Parse.			*/
/************************************************************************/
ST_RET sx_rip_xml_file (SX_DEC_CTRL *sxDecCtrl)
  {
  XML_Parser parser;		/* Expat parser control structure	*/
  ST_RET retcode = SD_SUCCESS;
  char *parseBuf;	/* temporary buffer to store text read from input file	*/

  if (!(parser = setup_expat (sxDecCtrl)))
    retcode = SD_FAILURE;	/* error already logged	in setup_expat	*/
  else
    {
    /* Allocate a reasonable size entity buffer.		*/
    /* This may be reallocated later if it is too small.	*/
    sxDecCtrl->entityBufSize = ENTITY_BUF_SIZE;
    sxDecCtrl->entityBuf = chk_malloc (sxDecCtrl->entityBufSize);
  
    parseBuf = chk_malloc (EXPAT_BUF_SIZE);

    /* In a loop, read the file one segment at a time & pass to parser.	*/
    for (;;)
      {
      int done;
      int len;
  
      len = fread(parseBuf, 1, EXPAT_BUF_SIZE, sxDecCtrl->fp);
      if (ferror(sxDecCtrl->fp))
        {
        fprintf(stderr, "Read error\n");
        retcode = SD_FAILURE;
        break;	/* stop now	*/
        }
      done = feof(sxDecCtrl->fp);
  
      if (XML_Parse(parser, parseBuf, len, done) == XML_STATUS_ERROR)
        {
        SXLOG_ERR2 ("XML parse error at line %d: %s\n",
                XML_GetCurrentLineNumber(parser),
                XML_ErrorString(XML_GetErrorCode(parser)));
        retcode = SD_FAILURE;
        break;	/* stop now	*/
        }
  
      if (done)
        break;
      }	/* end main loop	*/
    chk_free (parseBuf);
    chk_free (sxDecCtrl->entityBuf);
    }
  return (retcode);
  }
/************************************************************************/
/*			sx_rip_xml_mem					*/
/* Parse XML from a memory buffer. The caller must set			*/
/* the following members of sxDecCtrl before calling this function:	*/
/*   sxDecCtrl->xmlStart points to the buffer.				*/
/*   sxDecCtrl->xmlLen contains the length of the buffer.		*/
/* It simply sets up the parse and passes the buffer to XML_Parse.	*/
/************************************************************************/
ST_RET sx_rip_xml_mem (SX_DEC_CTRL *sxDecCtrl)
  {
  XML_Parser parser;		/* Expat parser control structure	*/
  int done = SD_TRUE;	/* indicates to parser that all data is being passed*/
  ST_RET retcode = SD_SUCCESS;

  if (!(parser = setup_expat (sxDecCtrl)))
    retcode = SD_FAILURE;	/* error already logged	in setup_expat	*/
  else
    {
    /* Allocate a reasonable size entity buffer.		*/
    /* This may be reallocated later if it is too small.	*/
    sxDecCtrl->entityBufSize = ENTITY_BUF_SIZE;
    sxDecCtrl->entityBuf = chk_malloc (sxDecCtrl->entityBufSize);

    /* Just pass data all at once to XML_Parse.	*/
    if (XML_Parse(parser, sxDecCtrl->xmlStart, sxDecCtrl->xmlLen, done) == XML_STATUS_ERROR)
      {
      SXLOG_ERR2 ("XML parse error at line %d: %s\n",
                XML_GetCurrentLineNumber(parser),
                XML_ErrorString(XML_GetErrorCode(parser)));
      retcode = SD_FAILURE;
      }
    chk_free (sxDecCtrl->entityBuf);
    }
  
  return (retcode);
  }
#endif	/* USE_EXPAT	*/

/************************************************************************/
/*			str_to_microsec					*/
/* Convert string to microseconds. String must start with '.' (e.g.	*/
/* ".1", ".123456", ".1234567890").					*/
/* NOTE: Returns 0 if string is NULL, if string does not start with	*/
/*       '.', or if conversion fails.					*/
/************************************************************************/
ST_VOID str_to_microsec (ST_CHAR *str, ST_LONG *microseconds)
  {
ST_CHAR strMicro[7];	/* 6 digits plus '\0'	*/
size_t len;
  if (str == NULL || str[0] != '.')	/* must start with '.'	*/	
    *microseconds = 0;
  else
    {
    /* copy digits after '.' and truncate to 6 digits	*/
    strncpy_safe (strMicro, str + 1, 6);
    len = strlen (strMicro);
    if (len < 6)
      strncat (strMicro, "000000", 6-len);	/* pad with 0's to 6 digits*/
    *microseconds = atol(strMicro);
    }
  return;
  }

