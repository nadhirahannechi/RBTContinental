import yaml
import os
from jinja2 import Template

# 🔹 Définir le chemin du fichier YAML
path = "./scripts_python"
os.chdir(path)

# 🔹 Charger le fichier `common.yml`
with open("common.yml", "r") as file:
    event_vars = yaml.safe_load(file)

# 🔹 Récupérer `event_data` du YAML
event_data = event_vars.get("event_data", {})

# 🔹 Mise à jour des valeurs (ce que tu faisais déjà)
event_data.update({
    "AEL": os.getenv("AEL", "default_AEL"),
    "Description": os.getenv("DESCRIPTION", "default_description"),
    "target_host": os.getenv("TARGET_HOST", "default_host")
})

# 🔹 Transformer `event_data` pour remplacer les variables Jinja
event_data_str = yaml.dump(event_data)  # Convertir en string YAML
template = Template(event_data_str)  # Utiliser Jinja pour remplacer {{ variables }}
event_data_final = yaml.safe_load(template.render(event_data))  # Convertir en dict

# 🔹 Affichage pour vérification
print("✅ Data après mise à jour et injection des variables Jinja :")
print(yaml.dump(event_data_final, default_flow_style=False))

# 🔹 Ajouter `event_data` mis à jour dans le payload de l’API
payload = {
    "extra_vars": {
        "event_data": event_data_final
    }
}
