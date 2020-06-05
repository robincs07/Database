select champion_name
from champ
where champion_id not in(
    select champion_id
from teamban
where match_id in(
        select match_id
from match_info
where version like '7.7%'
    )
)
order by champion_name asc;