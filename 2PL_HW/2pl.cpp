#include <bits/stdc++.h>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include <fstream>

using namespace std;

queue<string> q;
vector<int> nums;
int num;
vector<pthread_mutex_t> mutex1;
sem_t semaphore1, semaphore2, semaphore3;
vector<sem_t> semaphore4;
int n = 1;

void *job(void *unused)
{
    while (!q.empty())
    {
        sem_wait(&semaphore1);
        bool locked[num] = {false}, flg = false;
        string s;
        s = q.front();
        q.pop();
        if (q.empty())
        {
            flg = true;
        }
        int no = n;
        sem_post(&semaphore2);
        istringstream temps(s);
        string data;
        vector<string> equation;
        while (temps >> data)
        {
            equation.push_back(data);
        }
        string r_n;
        int regn = 0;
        r_n = equation[0];
        for (int i = 1; i < r_n.size(); i++)
        {
            regn = regn * 10 + r_n[i] - '0';
        }
        sem_wait(&semaphore4[no - 1]);
        pthread_mutex_lock(&mutex1[regn]);
        locked[regn] = true;
        int sum = 0;
        for (int i = 2; i < equation.size(); i++)
        {
            if (equation[i][0] == '$')
            {
                int tem = 0;
                for (int j = 1; j < equation[i].size(); j++)
                {
                    tem = tem * 10 + (equation[i][j] - '0');
                }
                if (!locked[tem])
                {
                    pthread_mutex_lock(&mutex1[tem]);
                    locked[tem] = true;
                }
            }
        }
        sem_post(&semaphore4[no]);
        for (int i = 1; i < equation.size(); i++)
        {
            if (equation[i] == "=" || equation[i] == "+")
            {
                i++;
                if (equation[i][0] == '$')
                {
                    int tem = 0;
                    for (int j = 1; j < equation[i].size(); j++)
                    {
                        tem = tem * 10 + (equation[i][j] - '0');
                    }
                    sum += nums[tem];
                }
                else
                {
                    sum += atoi(equation[i].c_str());
                }
            }
            else if (equation[i] == "-")
            {
                i++;
                if (equation[i][0] == '$')
                {
                    int tem = 0;
                    for (int j = 1; j < equation[i].size(); j++)
                    {
                        tem = tem * 10 + (equation[i][j] - '0');
                    }
                    sum -= nums[tem];
                }
                else
                {
                    sum -= atoi(equation[i].c_str());
                }
            }
        }
        nums[regn] = sum;
        for (int i = 0; i < num; i++)
        {
            if (locked[i])
            {
                pthread_mutex_unlock(&mutex1[i]);
            }
        }
        if (flg)
        {
            sem_post(&semaphore3);
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    time_t start, end;

    fstream(file);
    file.open(argv[2], ios::out | ios::trunc);
    sem_init(&semaphore2, 0, 0);
    sem_init(&semaphore3, 0, 0);
    sem_init(&semaphore1, 0, 0);

    string s;
    cin >> num;
    mutex1.resize(num);
    for (int i = 0; i < num; i++)
    {
        pthread_mutex_init(&mutex1[i], NULL);
    }

    for (int i = 0; i < num; i++)
    {
        int k;
        cin >> k;
        nums.push_back(k);
    }
    pthread_t thread[atoi(argv[1])];
    getline(cin, s);
    int count = 0;
    while (getline(cin, s))
    {
        q.push(s);
        count++;
    }
    semaphore4.resize(count + 1);
    for (int i = 0; i < count + 1; i++)
    {
        sem_init(&semaphore4[i], 0, 0);
    }
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        pthread_create(&thread[i], NULL, job, NULL);
    }
    sem_post(&semaphore4[0]);
    start = time(NULL);
    for (int i = 0; i < count; i++)
    {
        sem_post(&semaphore1);
        sem_wait(&semaphore2);
        n++;
    }
    end = time(NULL);
    sem_wait(&semaphore3);
    for (int i = 0; i < num; i++)
    {
        file << "$" << i << " = " << nums[i] << endl;
        pthread_mutex_destroy(&mutex1[i]);
    }
    file.close();

    cout << "Job assign time: " << (end - start) << "s" << endl;
    return 0;
}