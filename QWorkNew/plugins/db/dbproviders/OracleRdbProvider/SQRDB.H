/********************************************************************
 SQRDB.H - This is the include file for ODBC options that are
		   specific to the Oracle ODBC Driver for Rdb.


 (C) Copyright 1993,1995 By Oracle Corporation
*********************************************************************/

#ifndef __SQRDB
#define __SQRDB

/************************************************************************/
/* Oracle ODBC Driver for Rdb specific CONNECT Options					*/
/************************************************************************/

#define SQRDB_SSC_LOG			9001	 /* SQL/Services Client Logging */
#define SQRDB_BLOB_ACCESS		9004	 /* Specifies how binary data	*/
										 /* (blobs) are accessed.		*/

/************************************************************************/
/* Logging values associated with the SQRDB_SSC_LOG connect option		*/
/************************************************************************/

#define SQRDB_SSC_LOG_DISABLED	   0	/* 0 = Turn off logging.		*/
#define SQRDB_SSC_LOG_ASSOCIATION  1	/* To enable more than one type */
#define SQRDB_SSC_LOG_ROUTINE	   2	/* of logging, use the sum of	*/
#define SQRDB_SSC_LOG_PROTOCOL	   4	/* the desired logging values.	*/

/************************************************************************/
/* Values associated with the SQRDB_BLOB_ACCESS connect option			*/
/************************************************************************/

#define SQRDB_BLOB_ACCESS_TRANSPARENT	0	 /* Access is transparent to */
											 /* the application.		 */
#define SQRDB_BLOB_ACCESS_WITH_LISTS	1	 /* Access is controlled by	 */
											 /* the application via the	 */
											 /* use of list cursors.	 */

/************************************************************************/
/* Oracle ODBC Driver for Rdb specific STATEMENT Options				*/
/************************************************************************/

#define SQRDB_LIST_LEN			9002	 /* Maximum length of data that */
										 /* will be returned for a		*/
										 /* Segmented String.			*/
										 /* This option can be used to	*/
										 /* override the default segment*/
										 /* size of the string.			*/

#define SQRDB_CURSOR_TYPE		9003	 /* Type of cursor that will be */
										 /* declare when a SQL statement*/
										 /* is executed.				*/

/************************************************************************/
/* Cursor Types associated with the SQRDB_CURSOR_TYPE statement option	*/
/************************************************************************/

#define SQRDB_CURSOR_READ		  0		 /* Read						*/
#define SQRDB_CURSOR_UPDATE		  1		 /* Update						*/
#define SQRDB_CURSOR_INSERT		  2		 /* Insert						*/
#define SQRDB_CURSOR_UPDATE_ONLY  3		 /* Update Only					*/


#endif	/* #ifndef __SQRDB */
