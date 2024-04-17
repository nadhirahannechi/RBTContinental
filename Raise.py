def download_file(file_url):
    headers = {"PRIVATE-TOKEN": source_private_token}
    try:
        response = requests.get(f"{source_gitlab_url}/{file_url}", headers=headers, verify=False)
        response.raise_for_status()  # Lève une exception pour les erreurs HTTP
        return response.content
    except requests.exceptions.RequestException as e:
        print(f"Erreur lors du téléchargement du fichier depuis {file_url}: {e}")
        return None
