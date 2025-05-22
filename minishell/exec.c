/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/22 14:43:41 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pipe_count(t_exec *node)
{
    t_exec *new;
    int i;

    new = node;
    i = 0;
    while (new)
    {
        if(new->rank == 1)
            i++;
        new = new->next;
    }
    node->pipe = i;
}
void set_heredoc()
{

}

void set_redirect()
{
	
}

void	prep_exec(t_main *program)
{
	t_exec *current;
	t_executor **node;
	int i;
	int count;

	current = program->exec;
	count = 0;
	setting_sign(program);
	pipe_count(program->exec);
	i = program->exec->pipe + 2;
	node = malloc(sizeof(t_executor *) * (i));
	if (!node)
		return ; // exit 
	while (count < i)
	{
		node[i] = malloc(sizeof(t_executor *) * (i));
		if (!node[i])
		{
			while (--i > 0)
				free(node[i]);
			free(node);
		}
		node[i]->argv = set_argv(current, i);
		node[i]->infile = NULL;
		node[i]->outfile = NULL;
		node[i]->heredoc = NULL;
		node[i]->append_outfile = NULL;
		node[i]->pipe = program->exec->pipe;
		node[i]->next = NULL;
		current = current->next;
	}
	print_exec_list(program->exec->next);
}

char **set_argv(t_exec *temp, int i)
{
	t_exec *current;
    t_executor **node;
    int j;

	j = 0;
    current = temp;
    while (current)
    {
        while (current && current->rank == 4)
        {
            node[i]->argv[j] = ft_strdup(current->content);
            j++;
            current = current->next;
        }
		if (current && current->rank == 3)
        	set_heredoc();
		if (current && current->rank != 3 && current->rank != 1)
			set_redirect();
		
    }    
}


void	redirect_handle(t_executor *node)
{
    int fd;

    // Dosya yönlendirme işlemleri sadece child process'te yapılmalı
    if (node->infile) // Girdi dosyası varsa
    {
        fd = open(node->infile, O_RDONLY);
        if (fd < 0)
        {
            perror("minishell: open infile");
            return;
        }
        dup2(fd, STDIN_FILENO); // Girdi dosyasını stdin'e yönlendir
        close(fd);
    }
    
    if (node->outfile) // Çıktı dosyası varsa
    {
        fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("minishell: open outfile");
            return;
        }
        dup2(fd, STDOUT_FILENO); // Çıktı dosyasını stdout'a yönlendir
        close(fd);
    }
    
    if (node->append_outfile) // Çıktıya ekleme dosyası varsa
    {
        fd = open(node->append_outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
        {
            perror("minishell: open append outfile");
            return;
        }
        dup2(fd, STDOUT_FILENO); // Çıktıya eklemeyi stdout'a yönlendir
        close(fd);
    }
    
    if (node->heredoc) // Heredoc işlemi varsa
    {
        // Heredoc işlemine göre özel bir yönlendirme yapabilirsin.
        // Burada sadece placeholder olarak bırakıyorum:
        // heredoc_handle(node->heredoc);
    }
}

void main_execute(t_executor *exec)
{
    int pipefds[2];    // Pipe'dan veri geçirecek dosya descriptorları
    pid_t pid;         // Child process ID
    int prev_fd = 0;   // Önceki komutun çıktısını tutacak dosya descriptoru (ilk başta 0, stdin)
    t_executor *current = exec;

    // Komutlar arasında dolaşalım
    while (current != NULL)
    {
        // Eğer bir sonraki komut varsa, pipe aç
        if (current->next != NULL)
        {
            if (pipe(pipefds) == -1)
            {
                perror("pipe failed");
                exit(1);
            }
        }

        pid = fork(); // Her komut için child process oluşturuluyor

        if (pid == -1)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) // Child process
        {
            // Yönlendirmeleri uygula (input, output yönlendirmeleri)
            redirect_handle(current);

            // Eğer önceki komutun çıktısı bir pipe'a yönlendirilmişse
            if (prev_fd != 0)
            {
                dup2(prev_fd, STDIN_FILENO); // Önceki komutun çıktısını stdin'e yönlendir
                close(prev_fd);
            }

            // Eğer bu komut bir pipe'a veri gönderecekse
            if (current->next != NULL)
            {
                close(pipefds[0]); // Okuma ucu kapatılıyor (bu komutun stdout'u pipe'a yazacak)
                dup2(pipefds[1], STDOUT_FILENO); // stdout'u pipe'ın yazma ucuna yönlendir
            }

            // Eğer bu komut son komutsa, standart çıktıyı ekran veya dosyaya yönlendir
            if (current->next == NULL)
            {
                close(pipefds[1]); // Yazma ucu kapatılır
            }

            // Komutu execve ile çalıştır
            execve(current->argv[0], current->argv, NULL);
            // Eğer execve başarısız olursa
            perror("execve failed");
            exit(1);
        }
        else // Parent process
        {
            // Önceki pipe'ı kapatıyoruz çünkü sadece bu komutun stdout'u ile ilgileniyoruz
            if (prev_fd != 0)
                close(prev_fd);

            // Eğer son bir komut çalışıyorsa, son pipe'ı kapatıyoruz
            if (current->next != NULL)
                close(pipefds[1]);

            prev_fd = pipefds[0]; // Sonraki komutun girişine bu pipe'ı yönlendir

            current = current->next; // Sonraki komut için ilerle
        }
    }

    // Parent process, child'ları bekler
    while (wait(NULL) > 0)
        ;
}
