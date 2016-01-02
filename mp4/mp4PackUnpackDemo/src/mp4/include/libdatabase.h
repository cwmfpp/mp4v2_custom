/*******************************************************************
文件名	:database.h
公司	:天津天地伟业数码科技有限公司
创建者	:陶永亮
创建日期:2009-11-25
简要描述:数据库相关函数的原型
修改记录:2009-11-25,陶永亮，创建
		 2011-03-14,王松，修改接口名称
        
******************************************************************/

#ifndef _LIB_DATABASE_H_
#define _LIB_DATABASE_H_

#include "public_def.h"

#define DB_MAX_NUM  9



typedef enum
{
	COLUMN_INDEX = 0,
	COLUMN_CHN,
	COLUMN_FILETYPE,
	COLUMN_RECTYPE,
	COLUMN_DEVTYPE,
	COLUMN_RECATTR,
	COLUMN_NAME,
	COLUMN_SIZE,
	COLUMN_START,
	COLUMN_STOP,
	COLUMN_RESERVED,
	COLUMN_MAX
}TDbTableColumn;

int db_Init(int _iDisksExist);

/*
 * 函数名称：InitDB
 * 简要描述: 初始化数据库子模块
 * 输	  入：
 * 			可用磁盘，按位表示
 * 输	  出：
 * 			ERR_REC_SUCCESSFUL,操作成功
 * 			ERR_REC_FAILED,其他原因导致操作失败
 */
 
int db_IsInited(void);
/*
 * 函数名称：IsDBInited
 * 简要描述: 查询数据库模块是否已初始化
 * 输	  入：
 * 			无
 * 输	  出：
 * 			1,已初始化
 *			0，未初始化
 * 修改日志：
 * 			2009-11-26,陶永亮,从storage.c中挪到database.c
 */
 
int db_Exit(void);
/*
 * 函数名称：ExitDB
 * 简要描述: 退出数据库模块
 * 输	  入：
 * 			无
 * 输	  出：
 * 			ERR_REC_SUCCESSFUL,操作成功
 * 			ERR_REC_CLOSE_DB_FAILED,关闭数据库失败
 * 修改日志：
 * 			2009-11-26,陶永亮,从storage.c中挪到database.c
 */
int db_OpenDb(int _iDbNum);
/*
*简述:打开数据库
*输入:数据库对应磁盘编号
*/

int db_CloseDB(int _iDbNum);
/*
*简述:关闭数据库
*输入:数据库对应磁盘编号
*/
td_s32 db_Cleardb(int _iDbNum);
td_s32 db_ExecSql(const char* _pcSql, int _iDbNum);
/*
 * 函数名称：ExecSql
 * 简要描述: 执行SQL语句
 * 输	  入：
 * 			_pcSql,SQL语句，非空
 * 输	  出：
 * 			ERR_REC_SUCCESSFUL,操作成功
 * 			ERR_REC_QUERY_INDEX_FAILED,执行SQL语句失败
 * 修改日志：
 * 			2009-11-26,陶永亮,从storage.c中挪到database.c
 */
int db_GetRebuildState(int _iDbNum);
/*
*获取数据库重建索引状态
*输入:_iState	0,重建完成；1,重建开始
*/
int db_SetRebuildState(int _iState, int _iDbNum);
/*
*设置数据库重建索引状态
*输入:_iState	0,重建完成；1,重建开始
*/
int db_GetTable(const char* _pucSql, char*** _pppcResult, int* _piRow, int* _piColumn, int _iChn);
/*
 * 函数名称：GetTable
 * 简要描述: 从SQL语句查询记录
 * 输	  入：
 * 			_pucSql,SQL语句，非空
 *			_pppcResult, 用于传出结果，非空
 *			_piRow, 用于传出共有多少条记录
 *			_piColumn, 用于传出每条记录有几个字段
 * 输	  出：
 * 			ERR_REC_SUCCESSFUL,操作成功
 * 			ERR_REC_GET_TABLE_FAILED,执行SQL语句失败
 * 修改日志：
 * 			2009-11-26,陶永亮,从storage.c中挪到database.c
 */
int db_GetCountOrSum(const char* _pcSql, int _iChn);
/*
*简述:获取条目数或者统计列总和
*输入:
*		_pcSql:	sql语句，非空
*		_iChn:	通道号
*/
int db_GetTableEx(const char* _pcSql, TQueryResult** _ppstResult, int* _piRow, int* _piColumn, int _iChn, unsigned int _iDbNums, int _iMaxColNum);
/*
*简述:查询数据库
*/

int db_GetDB(void **_pstDB);
/*
 * 函数名称：db_GetDB
 * 简要描述: 对外函数，用于外部获取打开的数据库指针
 * 输	  入：
 *			_pstDB, 用于传出结果
 * 输	  出：
 * 			TD_SUCCESS,操作成功
 * 			TD_FAILURE,执行SQL语句失败
 * 修改日志：
 * 			2011-12-28,添加
 */
 int db_Sqlite3Exec(void *_pstDB, const char *sql, int (*callback)(void*,int,char**,char**), void *_CallBackParam, char **errmsg);
/*
 * 函数名称：db_Sqlite3Exec
 * 简要描述: 对外函数，用于外部调用数据库的sqlite3_exec函数
 * 输	  入：
 *			与sqlite3_exec()的输入值相同
 * 输	  出：
 * 			返回值与sqlite3_exec()的返回值相同
 * 修改日志：
 * 			2011-12-28,添加
 */
typedef void (*db_ChangeCallBack)(void);
int db_RegChangeCallBack(db_ChangeCallBack _CallBack);
int db_ExecSqlTrans(char** _pcSql, int _iCount,int _iDbNum);
int db_ExecSqlRight(const char* _pcSql, int _iDbNum);
/*
 * 函数名称：db_ExecSqlRight
 * 简要描述: 执行SQL语句,并需要立即执行，而不经过sql队列处理
 * 输	  入：
 * 			_pcSql,SQL语句，非空
 *			_iDbNum,数据库对应磁盘号
 * 输	  出：
 * 			ERR_REC_SUCCESSFUL,操作成功
 * 			ERR_REC_QUERY_INDEX_FAILED,执行SQL语句失败
 * 修改日志：
 * 			20121127，王松创建
 */

/*
*简要描述:注册标记数据库变化的回调函数
*/
int db_GetTableName(char* _pcTableName[]);
int db_GetRebuildState(int _iDbNum);
void db_FreeResultMem(TQueryResult* _pstResultMem);
TQueryResult *db_MallocResultMem(unsigned int _usSize);

#endif /*_DATABASE_H_*/


