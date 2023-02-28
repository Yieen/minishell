/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:39:46 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/28 14:30:29 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void disable_c()
{
	struct termios term;
	tcgetattr(0, &term);
	if (term.c_lflag & ECHOCTL)
		term.c_lflag ^= ECHOCTL;
	tcsetattr(0, TCSAFLUSH, &term);
}

void change_C()
{
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag ^= ECHOCTL;
	tcsetattr(0, TCSAFLUSH, &term);
}


void handdle_sig(int sig)
{
	if (sig == SIGINT)
		printf("handled\n");
		// fork();
	else
	{
		printf("exit\n");
		exit (0);
	}

}

// void handdle_sigquit(int sig)
// {
// 	printf("handled\n");
// 	exit(0);
// }

int main()
{
	disable_c();
	signal(SIGINT, handdle_sig);
	signal(SIGQUIT, handdle_sig);
	while (1)
	{}
}