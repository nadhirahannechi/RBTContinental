import requests

# Informations d'authentification pour le GitLab source (ancien GitLab)
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "YOUR_SOURCE_PRIVATE_TOKEN"

# Informations d'authentification pour le GitLab de destination (nouveau GitLab)
destination_gitlab_url = "https://destination-gitlab.com"
destination_private_token = "YOUR_DESTINATION_PRIVATE_TOKEN"

# Fonction pour récupérer la liste des utilisateurs d'un GitLab avec leurs ID
def get_users(gitlab_url, private_token):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/api/v4/users", headers=headers)
    users = response.json()
    return {user["username"]: user["id"] for user in users}

# Récupérer la liste des utilisateurs de l'ancien GitLab
old_users = get_users(source_gitlab_url, source_private_token)

# Récupérer la liste des utilisateurs du nouveau GitLab
new_users = get_users(destination_gitlab_url, destination_private_token)

# Créer un mapping entre les anciens et les nouveaux ID utilisateur basé sur les noms d'utilisateur
user_mapping = {}
for username, old_user_id in old_users.items():
    new_user_id = new_users.get(username)
    if new_user_id:
        user_mapping[old_user_id] = new_user_id

print("Mapping des utilisateurs créé avec succès :")
print(user_mapping)
