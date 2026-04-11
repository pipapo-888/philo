/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 14:07:21 by knomura           #+#    #+#             */
/*   Updated: 2026/04/11 14:50:11 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks_even(t_philo *p, t_data *data, t_rule rule)
{
	pthread_mutex_lock(p->right_fork);
	if (is_dead(data) || is_all_eaten(data))
	{
		pthread_mutex_unlock(p->right_fork);
		return (-1);
	}
	printf("%ld %d has taken a fork\n", t_ms() - rule.start_time, p->id);
	pthread_mutex_lock(p->left_fork);
	if (is_dead(data) || is_all_eaten(data))
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
		return (-1);
	}
	printf("%ld %d has taken a fork\n", t_ms() - rule.start_time, p->id);
	return (0);
}

static int	take_forks_odd(t_philo *p, t_data *data, t_rule rule)
{
	pthread_mutex_lock(p->left_fork);
	if (is_dead(data) || is_all_eaten(data))
	{
		pthread_mutex_unlock(p->left_fork);
		return (-1);
	}
	printf("%ld %d has taken a fork\n", t_ms() - rule.start_time, p->id);
	pthread_mutex_lock(p->right_fork);
	if (is_dead(data) || is_all_eaten(data))
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return (-1);
	}
	printf("%ld %d has taken a fork\n", t_ms() - rule.start_time, p->id);
	return (0);
}

static int	eat_meal(t_philo *p, t_data *data, t_rule rule)
{
	printf("%ld %d is eating\n", t_ms() - rule.start_time, p->id);
	pthread_mutex_lock(&p->eat_time);
	p->last_eat_time = t_ms();
	pthread_mutex_unlock(&p->eat_time);
	p->meal_count++;
	if (p->meal_count == rule.number_of_times_each_philo_eat)
	{
		pthread_mutex_lock(&data->all_ate_mtx);
		data->all_ate_count++;
		pthread_mutex_unlock(&data->all_ate_mtx);
	}
	usleep(rule.time_to_eat * 1000);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	return (0);
}

static int	sleep_and_think(t_philo *p, t_data *data, t_rule rule)
{
	if (is_dead(data) || is_all_eaten(data))
		return (1);
	printf("%ld %d is sleeping\n", t_ms() - rule.start_time, p->id);
	usleep(rule.time_to_sleep * 1000);
	if (is_dead(data) || is_all_eaten(data))
		return (1);
	printf("%ld %d is thinking\n", t_ms() - rule.start_time, p->id);
	if (rule.number_of_philo % 2 == 1 && rule.time_to_eat >= rule.time_to_sleep)
		usleep((rule.time_to_eat - rule.time_to_sleep + 1) * 1000);
	return (0);
}

int	do_cycle(t_philo *p, t_data *data, t_rule rule)
{
	if (p->id % 2 == 0)
	{
		if (take_forks_even(p, data, rule) == -1)
			return (1);
	}
	else
	{
		if (take_forks_odd(p, data, rule) == -1)
			return (1);
	}
	if (eat_meal(p, data, rule))
		return (1);
	if (sleep_and_think(p, data, rule))
		return (1);
	return (0);
}
