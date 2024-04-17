import os
import requests
import json
import re

# Ignorer les avertissements SSL
requests.packages.urllib3.disable_warnings(requests.packages.urllib3.exceptions.InsecureRequestWarning)

# Informations d'authentification pour l'instance GitLab
gitlab_url = "https://gitlab.com"
private_token = "VOTRE_TOKEN_PRIVÉ"
project_id = "ID_DU_PROJET"

# Répertoire pour enregistrer les fichiers et images téléchargés
download_directory = "downloads"

# Fonction pour télécharger un fichier depuis l'instance GitLab
def download_file(file_url, local_filename):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/{file_url}", headers=headers, verify=False)
    with open(local_filename, 'wb') as file:
        file.write(response.content)

# Lire les données de problèmes à partir de l'API GitLab
def get_issues_with_notes(project_id):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers, verify=False)
    issues = response.json()
    for issue in issues:
        notes_response = requests.get(f"{gitlab_url}/api/v4/projects/{project_id}/issues/{issue['id']}/notes", headers=headers, verify=False)
        notes = notes_response.json()
        issue['notes'] = notes
    return issues

# Créer le répertoire de téléchargement s'il n'existe pas
os.makedirs(download_directory, exist_ok=True)

# Télécharger les fichiers et images des notes des problèmes
issues = get_issues_with_notes(project_id)
for issue in issues:
    for note in issue['notes']:
        # Vérifier si la note contient des fichiers ou des images
        if 'attachments' in note:
            for attachment in note['attachments']:
                file_url = attachment['url']
                # Extraire le nom du fichier de l'URL
                filename = os.path.basename(file_url)
                # Télécharger le fichier dans le répertoire de téléchargement
                download_file(file_url, os.path.join(download_directory, filename))

print("Téléchargement terminé. Les fichiers et images ont été enregistrés dans le dossier de téléchargement.")
