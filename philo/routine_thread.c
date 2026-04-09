/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/09 20:08:07 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_data *data)
{
	int	flag;

	pthread_mutex_lock(&data->death_flag_mtx);
	flag = data->death_flag;
	pthread_mutex_unlock(&data->death_flag_mtx);
	return (flag);
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
		pthread_mutex_lock(p->left_fork);
		printf("%ld 1 has taken a fork\n", t_ms() - rule.start_time);
		while (!is_dead(data))
			usleep(100);
		pthread_mutex_unlock(p->left_fork);
		return (NULL);
	}
	if (p->id % 2 == 0)
		usleep(100);
	while (!is_dead(data))
	{
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				p->id);
			pthread_mutex_lock(p->left_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->right_fork);
				pthread_mutex_unlock(p->left_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				p->id);
			printf("%ld %d is eating\n", t_ms() - rule.start_time, p->id);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			usleep(rule.time_to_eat * 1000);
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
			{
				pthread_mutex_lock(&data->all_ate_mtx);
				data->all_ate_count++;
				pthread_mutex_unlock(&data->all_ate_mtx);
				return (NULL);
			}
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is sleeping\n", t_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep * 1000);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is thinking\n", t_ms() - rule.start_time, p->id);
		}
		else
		{
			pthread_mutex_lock(p->left_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->left_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				p->id);
			// タイミングとしてはここで待つわけだよね？
			pthread_mutex_lock(p->right_fork);
			if (is_dead(data))
			{
				pthread_mutex_unlock(p->left_fork);
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", t_ms() - rule.start_time,
				p->id);
			printf("%ld %d is eating\n", t_ms() - rule.start_time, p->id);
			pthread_mutex_lock(&p->eat_time);
			p->last_eat_time = t_ms();
			pthread_mutex_unlock(&p->eat_time);
			usleep(rule.time_to_eat * 1000);
			pthread_mutex_unlock(p->left_fork);
			pthread_mutex_unlock(p->right_fork);
			p->meal_count++;
			if (p->meal_count == rule.number_of_times_each_philo_eat)
			{
				pthread_mutex_lock(&data->all_ate_mtx);
				data->all_ate_count++;
				pthread_mutex_unlock(&data->all_ate_mtx);
				return (NULL);
			}
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is sleeping\n", t_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep * 1000);
			if (is_dead(data))
				return (NULL);
			printf("%ld %d is thinking\n", t_ms() - rule.start_time, p->id);
			// philoの数が貴数人の時　time to eat- time to sleepの時間だけ usleepする
		}
	}
	return (NULL);
}
