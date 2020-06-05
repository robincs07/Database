SELECT position, champion_name
FROM champ p, (
    select c.champion_id, m.position
    FROM (
        SELECT max(cnt) as maxcnt, position
        FROM (
            select champion_id, count(champion_id) as cnt, position
            from participant
            where match_id in (
                select match_id
                from match_info
                where duration BETWEEN 2400 AND 3000
            )
                and position !='DUO'
                and position != 'NONE'
                and position!='SOLO'
            group by position, champion_id
            order by position,  cnt desc
        )as c
        group by position
    )as m, (
        select champion_id, count(champion_id) as cnt, position
        from participant
        where match_id in (
                select match_id
            from match_info
            where duration BETWEEN 2400 AND 3000
            )
            and position !='DUO'
            and position != 'NONE'
            and position!='SOLO'
        group by position, champion_id
        order by position,  cnt desc
    )as c
    where m.maxcnt=c.cnt and m.position=c.position
) as f
where p.champion_id=f.champion_id;