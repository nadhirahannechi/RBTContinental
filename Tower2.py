import requests
import os
import time
import re
import json

# 🔹 Charger les variables d’environnement (comme dans le playbook)
incident_instanceid = os.getenv("incident_instanceid", "default_id")
iid = os.getenv("IID", "default_iid")
iid_extra = os.getenv("IID_EXTRA", "default_iid_extra")
target_host = os.getenv("TARGET_HOST", "localhost")
tower_org = os.getenv("TOWER_ORG", "default_org")
tower_env = os.getenv("TOWER_ENV", "test4")
tower_username = os.getenv("TOWER_USERNAME", "admin")
tower_password = os.getenv("TOWER_PASSWORD", "password")
ansible_path = os.getenv("ANSIBLE_PATH", "/path/to/ansible")

# 🔹 Déterminer l’environnement d’exécution
exec_env = os.getenv("EXEC_ENV", "LOCAL")

if exec_env == "TOWER":
    api_url = "https://your-tower-instance/api/v2/job_templates/your-template/launch/"
elif exec_env == "NOI":
    api_url = "https://your-noi-instance/api/run-job"
elif exec_env == "LOCAL":
    print("❌ LOCAL execution non supportée pour l’instant")
    exit(1)
else:
    print(f"❌ ENV {exec_env} non reconnu")
    exit(1)

# 🔹 Définir les headers et l'authentification pour l'API Tower
headers = {
    "Content-Type": "application/json"
}
auth = (tower_username, tower_password)

# 🔹 Construire le payload JSON pour l'API
payload = {
    "extra_vars": {
        "incident_instanceid": incident_instanceid,
        "IID": iid,
        "IID_EXTRA": iid_extra,
        "target_host": target_host,
        "TOWER_ORG": tower_org,
        "ANSIBLE_PATH": ansible_path
    }
}

# 🔹 Lancer l'exécution via API
response = requests.post(api_url, headers=headers, auth=auth, json=payload)

if response.status_code == 201:
    job_data = response.json()
    job_id = job_data.get("id")
    print(f"✅ Job lancé avec succès. ID: {job_id}")

    # 🔹 Suivre l’exécution du job
    job_url = f"https://your-tower-instance/api/v2/jobs/{job_id}/stdout/"
    
    while True:
        job_response = requests.get(job_url, headers=headers, auth=auth)
        if job_response.status_code == 200:
            job_output = job_response.text
            print(job_output)

            # 🔹 Vérifier si le job est terminé
            if "successful" in job_output or "failed" in job_output:
                print("✅ Job terminé.")
                break
        else:
            print("❌ Erreur lors de la récupération des logs")
        
        time.sleep(10)

else:
    print(f"❌ Échec du lancement du job. Erreur: {response.text}")
