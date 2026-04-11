/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/09 16:34:32 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rule
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philo_eat;
	long			start_time;
}					t_rule;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				meal_count;
	long			last_eat_time;
	pthread_mutex_t	eat_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	void			*data;
}					t_philo;

typedef struct s_data
{
	t_rule			rule_data;
	t_philo			*philos_data;
	pthread_mutex_t	*forks;
	pthread_t		thread;
	int				death_flag;
	pthread_mutex_t	death_flag_mtx;
	int				all_ate_count;
	pthread_mutex_t	all_ate_mtx;
}					t_data;

long				t_ms(void);
int					ft_atoi(char *str);
int					insert_rule(t_rule *rule, int ac, char **argv);
int					init_data(t_data *data);
void				*check_death(void *arg);
void				*routine(void *arg);
int					make_thread(t_data *data);
int					is_dead(t_data *data);
int					is_all_eaten(t_data *data);
int					do_cycle(t_philo *p, t_data *data, t_rule rule);

#endif