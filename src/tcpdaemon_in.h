#ifndef _H_TCPDAEMON_IN_
#define _H_TCPDAEMON_IN_

/*
 * tcpdaemon - TCP���ӹ����ػ�
 * author      : calvin
 * email       : calvinwilliams@163.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include "rbtree.h"
#include "LOGC.h"

#include "tcpdaemon.h"

#ifdef __cplusplus
extern "C" {
#endif

/* IOMP */
#define MAX_IOMP_EVENTS			1024

/* �ź���ֵ�ṹ */
union semun
{
	int		val ;
	struct semid_ds	*buf ;
	unsigned short	*array ;
	struct seminfo	*__buf ;
};

/* �ػ������ṹ */
typedef struct
{
	int		fd[ 2 ] ;
} PIPE_T ;

struct TcpdaemonAcceptedSession
{
	int		sock ;
	struct sockaddr	addr ;
	
	void		*io_multiplex_data_ptr ;
	struct rb_node	io_multiplex_data_ptr_rbnode ;
	
	int		begin_timestamp ;
	struct rb_node	begin_timestamp_rbnode ;
} ;

struct TcpdaemonServerEnvirment
{
	struct TcpdaemonEntryParameter	*p_para ;
	
	OBJECTHANDLE			so_handle ; /* ��̬��򿪾�� */
	func_tcpmain			*pfunc_tcpmain ; /* ��̬�����:ͨѶ����Э�鼰Ӧ�ô����ص����� */
	void				*param_tcpmain ; /* ��ڲ��� */
	int				listen_sock ; /* �����׽��� */
	struct sockaddr_in		listen_addr ; /* ���������ַ */
	
	PID_T				*pids ;
	PIPE_T				*alive_pipes ; /* �������̻�֪�������̻��ܵ�������˵�ǹ�������֪ͨ�������̽���������ܵ� */
					/* parent fd[1] -> child fd[0] */
	
	/* ��Instance-Fork����ģ��ʹ�� */
	int				process_count ;
	
	/* ��Leader-Follow���̳�ģ��ʹ�� */
	int				accept_mutex ; /* accept�ٽ��� */
	int				index ; /* ����������� */
	int				requests_per_process ; /* �������̵�ǰ�������� */
	
	/* ��IO-Multiplex���̳�ģ��ʹ�� */
	int				*epoll_array ;
	int				this_epoll_fd ;
	unsigned char			io_multiplex_event ;
	void				*io_multiplex_data_ptr ;
	struct rb_root			session_io_multiplex_data_ptr_rbtree ;
	struct rb_root			session_begin_timestamp_rbtree ;
	
	/* ��Leader-Follow�̳߳�ģ��ʹ�� */
	THANDLE_T			*thandles ;
	TID_T				*tids ;
} ;

int LinkTcpdaemonAcceptedSessionDataPtrTreeNode( struct TcpdaemonServerEnvirment *p_env , struct TcpdaemonAcceptedSession *p_session );
struct TcpdaemonAcceptedSession *QueryTcpdaemonAcceptedSessionDataPtrTreeNode( struct TcpdaemonServerEnvirment *p_env , struct TcpdaemonAcceptedSession *p_session );
void UnlinkTcpdaemonAcceptedSessionDataPtrTreeNode( struct TcpdaemonServerEnvirment *p_env , struct TcpdaemonAcceptedSession *p_session );

int LinkTcpdaemonAcceptedSessionBeginTimestampTreeNode( struct TcpdaemonServerEnvirment *p_env , struct TcpdaemonAcceptedSession *p_session );
struct TcpdaemonAcceptedSession *GetTimeoutAcceptedSession( struct TcpdaemonServerEnvirment *p_env , int now_timestamp );
void UnlinkTcpdaemonAcceptedSessionBeginTimestampTreeNode( struct TcpdaemonServerEnvirment *p_env , struct TcpdaemonAcceptedSession *p_session );

void DestroyTcpdaemonAcceptedSessionTree( struct TcpdaemonServerEnvirment *p_env );


#ifdef __cplusplus
}
#endif

#endif
