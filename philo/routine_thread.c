/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/11 13:44:52 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_data *data)
{
	int	flag;

	pthread_mutex_lock(&data->death_flag_mtx);
	flag = data->death_flag;
	pthread_mutex_unlock(&data->death_flag_mtx);
	return (flag);
}

static void	routine_solo(t_philo *p, t_data *data, t_rule rule)
{
	pthread_mutex_lock(p->left_fork);
	printf("%ld 1 has taken a fork\n", t_ms() - rule.start_time);
	while (!is_dead(data))
		usleep(100);
	pthread_mutex_unlock(p->left_fork);
}

void	*routine(void *arg)
{
	t_philo	*p;
	t_data	*data;
	t_rule	rule;

	p = (t_philo *)arg;
	data = p->data;
	rule = data->rule_data;
	if (rule.number_of_philo == 1)
	{
		routine_solo(p, data, rule);
		return (NULL);
	}
	if (p->id % 2 == 0)
		usleep(100);
	while (!is_dead(data))
		if (do_cycle(p, data, rule))
			return (NULL);
	return (NULL);
}
