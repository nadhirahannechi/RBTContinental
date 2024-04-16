import requests
import json
from requests.packages.urllib3.exceptions import InsecureRequestWarning

# Ignorer les avertissements SSL
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

# Informations d'authentification pour le GitLab source
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "YOUR_SOURCE_PRIVATE_TOKEN"

# Mapping des utilisateurs entre les deux GitLab (ancien nom d'utilisateur -> nouveau nom d'utilisateur)
user_mapping = {
    "old_username1": "new_username1",
    "old_username2": "new_username2",
    # Ajoutez tous les mappings nécessaires
}

# Fonction pour récupérer les issues d'un projet du GitLab source
def get_issues(project_id):
    headers = {"PRIVATE-TOKEN": source_private_token}
    response = requests.get(f"{source_gitlab_url}/api/v4/projects/{project_id}/issues?include_merge_requests=false&include_subscribed=false", headers=headers, verify=False)
    issues = response.json()
    return issues

# Fonction pour obtenir l'ID d'utilisateur correspondant dans le GitLab de destination
def get_user_id(username):
    # Ajoutez votre propre logique pour obtenir l'ID d'utilisateur correspondant dans le nouveau GitLab
    pass

# Parcourir la liste des projets du GitLab source
all_issues = []
for project_id in [1, 2, 3]:  # Remplacez [1, 2, 3] par les IDs de projets nécessaires
    # Récupérer les issues du projet source
    issues = get_issues(project_id)
    
    # Parcourir les issues
    for issue in issues:
        # Créer une nouvelle structure de données pour stocker les informations de l'issue
        new_issue = {
            "title": issue["title"],
            "description": issue["description"],
            "author": user_mapping.get(issue["author"]["username"], None),
            "assignees": [user_mapping.get(assignee["username"], None) for assignee in issue["assignees"]],
            "created_at": issue["created_at"],
            "updated_at": issue["updated_at"],
            "notes": []
        }
        
        # Récupérer les notes de l'issue
        notes_response = requests.get(f"{source_gitlab_url}/api/v4/projects/{project_id}/issues/{issue['iid']}/notes", headers=headers, verify=False)
        notes = notes_response.json()
        
        # Ajouter les notes à la nouvelle structure de données
        for note in notes:
            new_note = {
                "body": note["body"],
                "author": user_mapping.get(note["author"]["username"], None),
                "created_at": note["created_at"]
            }
            new_issue["notes"].append(new_note)
        
        # Ajouter l'issue à la liste des issues
        all_issues.append(new_issue)

# Enregistrer les issues dans un fichier JSON
with open("issues_data.json", "w") as json_file:
    json.dump(all_issues, json_file)

print("Les issues ont été enregistrées dans le fichier issues_data.json avec succès.")
