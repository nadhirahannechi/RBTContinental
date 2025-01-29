import subprocess
import os
import sys
import re

def run_ansible_playbook(playbook, inventory, vault_password_file, iid, extra_vars, target_host):
    """
    Exécute un playbook Ansible avec les mêmes paramètres que Netcool Tools.

    :param playbook: Chemin du playbook Ansible
    :param inventory: Chemin du fichier d'inventaire
    :param vault_password_file: Fichier de mot de passe Ansible Vault
    :param iid: ID de l'incident (passé en extra_vars)
    :param extra_vars: Variables supplémentaires en JSON (ex: '{"key": "value"}')
    :param target_host: Hôte cible pour l'exécution du playbook
    :return: Code de retour d'Ansible
    """

    # Construire la commande Ansible avec les mêmes options que Netcool Tools
    cmd = [
        "ansible-playbook",
        "-i", inventory,
        playbook,
        "--vault-password-file", vault_password_file,
        "-e", f"incident_instanceid={iid}",
        "-e", f"target_host={target_host}",
        "-e", extra_vars  # Extra vars en format JSON ou YAML
    ]

    try:
        print(f"✅ Exécution du playbook : {playbook} avec iid={iid}, target_host={target_host}")
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        
        print("✅ Playbook exécuté avec succès !")
        print(result.stdout)

        # Extraire l'ID du job depuis la sortie Ansible
        pattern = r"Job Id[:=]\s*(\d+)"
        match = re.search(pattern, result.stdout)
        if match:
            job_id = match.group(1)
            launched_url = f"https://your-ci-url/jobs/{job_id}/stdout"
            print(f"🔗 Lien du job: {launched_url}")
        
        return result.returncode

    except subprocess.CalledProcessError as e:
        print("❌ Erreur lors de l'exécution du playbook !")
        print(e.stderr)
        sys.exit(e.returncode)

if __name__ == "__main__":
    # Récupération des variables depuis le pipeline CI/CD
    PLAYBOOK = "playbooks/ExecuteInstructionWithoutNetcool.yml"
    INVENTORY = "inventory/org/Test-env-inventory.yml"
    VAULT_PASSWORD_FILE = "../vault/ansible_vault_noprod.txt"

    IID = os.getenv("IID", "default_iid")  # Identifiant d'incident
    EXTRA_VARS = os.getenv("EXTRA_VARS", "{}")  # Variables supplémentaires sous format JSON
    TARGET_HOST = os.getenv("TARGET_HOST", "default_host")  # Hôte cible

    # Exécuter le playbook
    exit_code = run_ansible_playbook(PLAYBOOK, INVENTORY, VAULT_PASSWORD_FILE, IID, EXTRA_VARS, TARGET_HOST)
    sys.exit(exit_code)
