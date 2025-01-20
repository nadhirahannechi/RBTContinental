import os

def get_instructions_informations(instruction, org):
    instruction_test_list = []

    # Lecture des variables d'environnement
    incident_instanceid = os.getenv("INCIDENT_INSTANCEID", "default_value")
    iid = os.getenv("IID", "default_value")
    iid_extra = os.getenv("IID_EXTRA", "default_value")
    target_host = os.getenv("TARGET_HOST", "default_value")

    # Validation des données
    if not incident_instanceid or not iid or not target_host:
        print("Certaines variables nécessaires sont manquantes.")
        return []

    # Ajout des instructions à la liste pour traitement
    instruction_test_list.append({
        "instruction": instruction,
        "incident_instanceid": incident_instanceid,
        "iid": iid,
        "iid_extra": iid_extra,
        "target_host": target_host
    })

    print(f"Liste des instructions à tester : {instruction_test_list}")
    return instruction_test_list
  
