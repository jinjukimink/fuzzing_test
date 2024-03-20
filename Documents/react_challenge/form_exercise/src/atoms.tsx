import { atom,selector } from "recoil";

// export enum Categories{
//     "TODO"="TODO",
//     "DOING"="DOING",
//     "DONE"="DONE",
// }
//type categories= "TODO"|"DONE"|"DOING";
//export const Categories=["TODO","DOING","DONE"] as const;

// 카테고리를 문자열 배열로 정의합니다.
export interface ICategory{
    //id:number;
    category:string;
}

export const categoryState=atom({
    key:"category",
    //default:{id:1,category:"TODO"},
    //default:{category:"TODO"},
    default:"TODO",
});

export const categoriesArr = atom<string[]>({
  key: "categories",
  //default: [{id:1,category:"TODO"},{id:2,category:"DOING"},{id:3,category:"DONE"}]
//   default:[
//     {category:"TODO"},{category:"DOING"},{category:"DONE"}],
    default:["TODO","DOING","DONE"],
});

export interface IToDoProps{//배열 안의 요소가 다음의 인터페이스를 가진다.
    text:string;
    id:number;
    category:string; 
}

export const toDoState=atom<IToDoProps[]>({//toDoState는 배열이다 배열
    key:"toDo",
    default:[],
})

export interface ICategoryProps{
    text:string;
}

export const toDoSelector=selector({
    key:"toDoSelector",
    get:({get})=>{
        const fullArr=get(toDoState)//atom 값을 받아야 하지
        const categoryInfo=get(categoryState);//값을 받아와서
        return fullArr.filter((item) => item.category === categoryInfo);
    }
}) 