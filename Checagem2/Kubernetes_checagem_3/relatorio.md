# Relatório de Desenvolvimento - Projeto Flask Monte Carlo

Este relatório detalha as etapas de desenvolvimento, deploy e modificações realizadas na aplicação Flask Monte Carlo, que estima o valor de π e outras áreas geométricas utilizando simulações de Monte Carlo, com suporte para execução local via Docker e distribuída em Kubernetes.

---

## 1. Documentação / README

### 1.1. Explicação de como buildar e rodar a aplicação Docker

Para buildar a imagem Docker da aplicação, navegue até o diretório raiz do projeto onde se encontram o `Dockerfile` e o `app.py`. Em seguida, execute o comando:

```bash
docker build -t flask-montecarlo:latest .

````

Após o build bem-sucedido da imagem, você pode rodar a aplicação em um container Docker localmente, mapeando a porta 8080 do container para a porta 8080 do seu host:

``` bash
docker run -p 8080:8080 flask-montecarlo

```

Para verificar se a imagem foi criada e está disponível, você pode usar:

``` bash
docker images | grep flask-montecarlo

```

### 1.2. Passos para deploy no Kubernetes

O deploy da aplicação no Kubernetes é realizado através do manifesto `kube-flask-montecarlo.yaml`. Antes de aplicar o manifesto, é crucial garantir que a imagem Docker esteja disponível no ambiente do Minikube (ou do cluster Kubernetes).

1.  **Iniciar o Minikube (se não estiver rodando):**
    Quando o PC é reiniciado, o cluster Minikube para. Para iniciá-lo novamente, utilize:
    
    ``` bash
    minikube start
    
    ```
    
    Confirme o status com `minikube status`.

2.  **Carregar a imagem Docker para o Minikube:**
    Para evitar o erro `ErrImageNeverPull`, a imagem `flask-montecarlo:latest` deve ser carregada no cache do Minikube. Alternativamente, pode-se buildar a imagem diretamente no ambiente Docker do Minikube.
    
      * **Opção 1 (Recomendada): Carregar imagem local para o Minikube**
        
        ``` bash
        minikube image load flask-montecarlo:latest
        
        ```
        
        Após carregar, se o deployment já existia, pode ser necessário reiniciar os pods para que eles peguem a nova imagem:
        
        ``` bash
        kubectl rollout restart deployment flask-montecarlo-deployment
        
        ```
    
      * **Opção 2: Buildar diretamente dentro do Minikube**
        
        ``` bash
        eval $(minikube docker-env)
        docker build -t flask-montecarlo:latest .
        
        ```
        
        (Após este comando, desfaça o `eval` com `eval $(minikube docker-env -u)` se quiser voltar ao Docker local).

3.  **Aplicar o manifesto Kubernetes:**
    Navegue até o diretório que contém o arquivo `kube-flask-montecarlo.yaml` e aplique-o:
    
    ``` bash
    kubectl apply -f kube-flask-montecarlo.yaml
    
    ```
    
    Este manifesto cria um `Deployment` com 3 réplicas (`replicas: 3`) e um `Service` do tipo `LoadBalancer` (que em Minikube se comporta como `NodePort` ou requer `minikube tunnel` em algumas configurações) para expor a aplicação.

4.  **Verificar o status dos pods e do serviço:**
    
    ``` bash
    kubectl get pods
    kubectl get svc
    
    ```
    
    Garanta que os pods estejam com `STATUS` como `Running` e que o `Service` `flask-montecarlo-service` esteja com o `PORT(S)` configurado (ex: `8080:xxxxx/TCP`).

5.  **Acessar a aplicação no Kubernetes:**
    Se o `EXTERNAL-IP` do `Service` estiver `<pending>` (comum no Minikube sem `minikube tunnel`), utilize o comando `minikube service` para obter a URL de acesso:
    
    ``` bash
    minikube service flask-montecarlo-service --url
    
    ```
    
    Copie a URL fornecida para acessar os endpoints.

### 1.3. Lista de endpoints disponíveis com exemplos de chamadas

A aplicação Flask expõe os seguintes endpoints:

  * **`/docker-info`**
    Retorna informações do container Docker atual (hostname, diretório de trabalho e algumas variáveis de ambiente).
    
      * **Exemplo de chamada:**
        ``` bash
        # Se rodando em Docker local
        curl [http://127.0.0.1:8080/docker-info](http://127.0.0.1:8080/docker-info)
        
        # Se rodando em Kubernetes (após port-forward ou minikube service --url)
        curl http://<URL_DO_SERVICO>/docker-info
        
        ```

  * **`/montecarlo/<n>`**
    Executa uma simulação de Monte Carlo com `n` amostras para estimar o valor de π em uma única CPU (dentro de um container/pod).
    
      * **Exemplo de chamada:**
        ``` bash
        curl http://<URL_DO_SERVICO>/montecarlo/1000000
        
        ```

  * **`/montecarlo-distributed/<n>`**
    Executa uma simulação de Monte Carlo distribuída para estimar π. O total de `n` amostras é dividido entre os pods do deployment. Cada pod retorna uma estimativa parcial. Para o valor final, é necessário agregar os resultados de todos os pods.
    
      * **Exemplo de chamada:**
        ``` bash
        curl http://<URL_DO_SERVICO>/montecarlo-distributed/1000000
        
        ```
      * **Para agregar os resultados**, pode-se usar o script `montecarlo_aggregator.py` (vide seção 1.4).

  * **`/montecarlo-square/<n>`**
    (Modificação implementada) Estima a área de um quadrado inscrito em um círculo de raio 1 usando a simulação de Monte Carlo com `n` amostras. Retorna a contagem de pontos dentro do círculo e do quadrado, a proporção e a área estimada.
    
      * **Exemplo de chamada:**
        ``` bash
        curl http://<URL_DO_SERVICO>/montecarlo-square/1000000
        
        ```

  * **`/montecarlo-square-distributed/<n>`**
    (Modificação implementada) Versão distribuída da simulação de Monte Carlo para a área do quadrado inscrito. Cada pod calcula uma parte das amostras, retornando uma estimativa parcial. Os resultados devem ser agregados para obter a estimativa final.
    
      * **Exemplo de chamada:**
        ``` bash
        curl http://<URL_DO_SERVICO>/montecarlo-square-distributed/1000000
        
        ```
      * **Para agregar os resultados**, pode-se usar o script `montecarlo_aggregator.py` (vide seção 1.4).

  * **`/montecarlo-triangle/<n>`**
    (Modificação implementada) Estima a área de um triângulo (com vértices (0,0), (1,0), (0,1)) usando a simulação de Monte Carlo com `n` amostras. Retorna a área estimada e a área esperada (0.5).
    
      * **Exemplo de chamada:**
        ``` bash
        curl http://<URL_DO_SERVICO>/montecarlo-triangle/1000000
        
        ```

### 1.4. Explicação da modificação feita

Foi adicionado um novo endpoint para a aplicação Flask, `montecarlo-square/<n>`, que realiza uma variação da simulação de Monte Carlo. Este endpoint tem como objetivo estimar a área de um quadrado inscrito em um círculo de raio 1. A lógica envolve gerar pontos aleatórios dentro de um quadrado maior que engloba o círculo e o quadrado inscrito, e então verificar a proporção de pontos que caem dentro das respectivas formas.

Além disso, para demonstrar a capacidade de distribuir essa nova simulação, foi criado o endpoint `/montecarlo-square-distributed/<n>`, que segue a mesma lógica de divisão de amostras entre os pods, semelhante ao `/montecarlo-distributed/<n>`.

O script `montecarlo_aggregator.py` é utilizado para consolidar os resultados parciais obtidos dos pods ao usar os endpoints distribuídos (`/montecarlo-distributed/<n>` e `/montecarlo-square-distributed/<n>`). Ele faz requisições a cada pod (via o Kubernetes Service) e calcula a média das estimativas parciais para fornecer o resultado final.

**Exemplo de `montecarlo_aggregator.py` (adaptado para incluir o novo endpoint distribuído, se aplicável):**

``` python
import requests
import os

# URL base do Service
SERVICE_HOST = os.getenv("SERVICE_HOST", "127.0.0.1")
SERVICE_PORT =

```
