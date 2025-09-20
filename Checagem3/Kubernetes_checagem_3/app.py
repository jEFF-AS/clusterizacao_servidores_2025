import os
import socket
import random
from flask import Flask, jsonify, request

app = Flask(__name__)

# ---------------------------
# Função de simulação Monte Carlo
# ---------------------------
def monte_carlo_pi(num_samples: int) -> float:
    inside_circle = 0
    for _ in range(num_samples):
        x, y = random.random(), random.random()
        if x*x + y*y <= 1.0:
            inside_circle += 1
    return (4.0 * inside_circle) / num_samples


# ---------------------------
# Endpoints
# ---------------------------

@app.route('/')
def index():
    return jsonify({
        "message": "Olá do app Flask + Kubernetes!",
        "endpoints": [
            "/docker-info",
            "/montecarlo/<n>",
            "/montecarlo-distributed/<n>"
        ]
    })


@app.route('/docker-info')
def docker_info():
    """Retorna informações do container Docker atual"""
    info = {
        "hostname": socket.gethostname(),
        "cwd": os.getcwd(),
        "env": dict(list(os.environ.items())[:10])  # só os 10 primeiros envs
    }
    return jsonify(info)


@app.route('/montecarlo/<int:n>')
def montecarlo_single(n):
    """Monte Carlo single CPU (local, dentro de 1 container)"""
    pi_estimate = monte_carlo_pi(n)
    return jsonify({
        "samples": n,
        "pi_estimate": pi_estimate,
        "mode": "single-cpu"
    })


@app.route('/montecarlo-distributed/<int:n>')
def montecarlo_distributed(n):
    """
    Monte Carlo distribuído: divide o total de amostras (n)
    entre todos os pods (réplicas) do deployment.
    
    Estratégia: 
      - usa env var POD_REPLICAS para saber número de réplicas
      - usa env var POD_INDEX para simular ID do pod
      - cada pod executa sua parte
    """
    replicas = int(os.getenv("POD_REPLICAS", "1"))
    pod_index = int(os.getenv("POD_INDEX", "0"))

    # dividir amostras
    per_pod = n // replicas
    pi_estimate = monte_carlo_pi(per_pod)

    return jsonify({
        "samples_total": n,
        "samples_this_pod": per_pod,
        "replicas": replicas,
        "pod_index": pod_index,
        "pi_partial": pi_estimate,
        "note": "Resultados devem ser agregados fora (ex: via Service + client aggregator)."
    })

@app.route('/montecarlo-square/<int:n>')
def montecarlo_square(n):
    """
    Estima a área de um quadrado inscrito em um círculo de raio 1 usando Monte Carlo.
    Quadrado inscrito: |x| <= 1/sqrt(2), |y| <= 1/sqrt(2)
    """
    inside_square = 0
    inside_circle = 0
    for _ in range(n):
        x, y = random.uniform(-1, 1), random.uniform(-1, 1)
        if x*x + y*y <= 1.0:
            inside_circle += 1
            if abs(x) <= (2**-0.5) and abs(y) <= (2**-0.5):
                inside_square += 1

    # proporção de pontos dentro do quadrado em relação ao círculo
    if inside_circle > 0:
        proportion = inside_square / inside_circle
    else:
        proportion = 0.0

    # área estimada do quadrado
    estimated_area = proportion * 3.14159  # área do círculo = pi*r² (r=1)

    return jsonify({
        "samples": n,
        "points_inside_circle": inside_circle,
        "points_inside_square": inside_square,
        "proportion_square_circle": proportion,
        "estimated_area": estimated_area
    })

# ---------------------------
# Função de simulação Monte Carlo para área de um triângulo
# ---------------------------
def monte_carlo_triangle(num_samples: int) -> float:
    """
    Estima a área de um triângulo usando Monte Carlo.
    Vamos usar o triângulo com vértices (0,0), (1,0), (0,1).
    A área exata é 0.5.
    """
    inside_triangle = 0
    for _ in range(num_samples):
        x, y = random.random(), random.random()
        if x + y <= 1:  # condição para estar dentro do triângulo
            inside_triangle += 1
    # Área do quadrado é 1, então a área estimada é só a proporção de pontos dentro
    return inside_triangle / num_samples


# ---------------------------
# Novo Endpoint
# ---------------------------
@app.route('/montecarlo-triangle/<int:n>')
def montecarlo_triangle_endpoint(n):
    """Monte Carlo para estimar área de um triângulo"""
    area_estimate = monte_carlo_triangle(n)
    return jsonify({
        "samples": n,
        "area_estimate": area_estimate,
        "expected_area": 0.5,
        "mode": "triangle"
    })



# ---------------------------
# Run
# ---------------------------
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)

