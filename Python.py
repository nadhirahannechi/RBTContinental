import requests

# Informations d'authentification pour le GitLab source
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "YOUR_SOURCE_PRIVATE_TOKEN"

# Informations d'authentification pour le GitLab de destination
destination_gitlab_url = "https://destination-gitlab.com"
destination_private_token = "YOUR_DESTINATION_PRIVATE_TOKEN"

# Mapping des utilisateurs entre les deux GitLab (ancien nom d'utilisateur -> nouveau nom d'utilisateur)
user_mapping = {
    "old_username1": "new_username1",
    "old_username2": "new_username2",
    # Ajoutez tous les mappings nécessaires
}

# Fonction pour récupérer les issues d'un projet du GitLab source
def get_issues(project_id):
    headers = {"PRIVATE-TOKEN": source_private_token}
    response = requests.get(f"{source_gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers)
    issues = response.json()
    return issues

# Fonction pour obtenir l'ID d'utilisateur correspondant dans le GitLab de destination
def get_user_id(username):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.get(f"{destination_gitlab_url}/api/v4/users?username={username}", headers=headers)
    user = response.json()
    if user:
        return user[0]["id"]
    else:
        return None

# Parcourir la liste des projets du GitLab source
for project_id in [1, 2, 3]:  # Remplacez [1, 2, 3] par les IDs de projets nécessaires
    # Récupérer les issues du projet source
    issues = get_issues(project_id)
    
    # Parcourir les issues et les créer sur le GitLab de destination
    for issue in issues:
        title = issue["title"]
        description = issue["description"]
        author_id = get_user_id(user_mapping.get(issue["author"]["username"], None))
        assignee_ids = [get_user_id(user_mapping.get(assignee["username"], None)) for assignee in issue["assignees"]]
        created_at = issue["created_at"]
        updated_at = issue["updated_at"]
        
        # Créer l'issue sur le GitLab de destination
        # (à compléter avec votre propre logique pour la création des issues)
        
        print(f"L'issue '{title}' du projet '{project_id}' a été migrée avec succès.")
