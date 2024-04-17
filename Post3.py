import requests
import json
import re
import os

# Ignorer les avertissements SSL
requests.packages.urllib3.disable_warnings(requests.packages.urllib3.exceptions.InsecureRequestWarning)

# Informations d'authentification pour les instances GitLab source et destination
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "VOTRE_TOKEN_PRIVÉ_SOURCE"

destination_gitlab_url = "https://destination-gitlab.com"
destination_private_token = "VOTRE_TOKEN_PRIVÉ_DESTINATION"

# Définir l'identifiant du projet dans l'instance GitLab de destination
project_id = "ID_DU_PROJET_DESTINATION"

# Répertoire pour enregistrer les pièces jointes téléchargées
attachments_directory = "attachments"

# Fonction pour créer un problème dans l'instance GitLab de destination
def create_issue(issue_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers, json=issue_data, verify=False)
    created_issue = response.json()
    return created_issue

# Fonction pour créer une note pour un problème dans l'instance GitLab de destination
def create_note(issue_iid, note_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues/{issue_iid}/notes", headers=headers, json=note_data, verify=False)
    created_note = response.json()
    return created_note

# Fonction pour télécharger un fichier à partir de l'instance GitLab source
def download_file(file_url):
    headers = {"PRIVATE-TOKEN": source_private_token}
    response = requests.get(f"{source_gitlab_url}/{file_url}", headers=headers, verify=False)
    return response.content

# Fonction pour télécharger une image à partir de l'instance GitLab source
def download_image(image_url):
    headers = {"PRIVATE-TOKEN": source_private_token}
    response = requests.get(f"{source_gitlab_url}/{image_url}", headers=headers, verify=False)
    return response.content

# Fonction pour télécharger les pièces jointes des notes
def download_attachments_from_notes(notes):
    attachments = []
    for note in notes:
        if "attachments" in note:
            for attachment in note["attachments"]:
                attachment_content = download_file(attachment["url"])
                attachment_filename = os.path.join(attachments_directory, attachment["filename"])
                with open(attachment_filename, "wb") as file:
                    file.write(attachment_content)
                attachments.append({"filename": attachment["filename"], "local_path": attachment_filename})
    return attachments

# Lire les données de problème à partir du fichier JSON
with open("issues_data.json", "r") as json_file:
    all_issues = json.load(json_file)

# Loop à travers les problèmes
for issue_data in all_issues:
    # Télécharger les images référencées dans la description du problème
    description = issue_data.get("description", "")
    if description:
        image_urls = re.findall(r"!\[.*?\]\((.*?)\)", description)
        for image_url in image_urls:
            image_content = download_image(image_url)
            image_filename = os.path.join(attachments_directory, os.path.basename(image_url))
            with open(image_filename, "wb") as file:
                file.write(image_content)
            # Mettre à jour l'URL de l'image dans la description
            description = description.replace(image_url, f"/uploads/{image_filename}")
        issue_data["description"] = description
    
    # Télécharger les pièces jointes des notes
    attachments = download_attachments_from_notes(issue_data.get("notes", []))
    
    # Créer le problème dans l'instance GitLab de destination
    created_issue = create_issue(issue_data)
    
    # Créer les notes pour le problème dans l'instance GitLab de destination
    for note_data in issue_data.get("notes", []):
        # Télécharger les pièces jointes des notes
        attachments = download_attachments_from_notes([note_data])
        # Mettre à jour les URLs des pièces jointes dans le contenu de la note
        if "attachments" in note_data:
            for attachment in attachments:
                note_data["body"] = note_data["body"].replace(attachment["filename"], f"/uploads/{attachment['local_path']}")
        # Créer la note pour le problème dans l'instance GitLab de destination
        create_note(created_issue["iid"], note_data)

print("Les données des problèmes ont été transférées avec succès vers la nouvelle instance de GitLab.")
