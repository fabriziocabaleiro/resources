/**
 * \file   resfunctions.h
 * \author fabrizio
 * \brief  Define all the functions used by a resources to get the data that is
 * going to be store and graph
 * \date July 19, 2013, 7:09 PM
 */

#ifndef RESFUNCTIONS_H
#define	RESFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "resinfo.h"
#include "rrdtool.h"
#include "log.h"

/**
 * \brief Assign a corresponding function to every single element of the
 * resources list, this function it set the pointer to function attribute in
 * the structure to the corresponding function
 * \param head The head of the resources list
 * \return VOID
 */
void assign_func(ri *head);

/**
 * \brief Check if every resource is done fetching it's data 
 * \param head The head of the resources info list 
 * \return ZERO if any resource in the resources list is not done, ONE if all
 * resources are done fetching it's data
 */
int  ri_done(ri *head);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_cpu(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_cpu(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_net(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_net(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_disk(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_disk(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_uptime(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_uptime(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_mem(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_mem(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_swap(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_swap(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_all_users(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_all_users(ri *node, char *data);

/**
 * \brief Get the file pointer from the corresponding command and assign it to
 * the fd attribute in the giving resource structure
 * \param node A resources info node
 * \return ZERO if success, other value if failure
 */
int cmd_current_users(ri *node);

/**
 * \brief Read from the pointer to file "pf" the content of the executed command
 * and write the content in rrdtool format to the giving char*
 * \param node A resources info node to get the CPU data
 * \param data A char* to store the generated from the executed command
 * \return Zero if the command succeeded and other value if error.
 */
int get_current_users(ri *node, char *data);

#endif	/* RESFUNCTIONS_H */

